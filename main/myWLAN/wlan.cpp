// wlan.cpp

#include "wlan.h"
#include "myConfig/config.h"
#include "myDebug/debug.h"
#include "myWatchdog/watchdog_tasks.h"

#include "esp_task_wdt.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "DEBUG-WLAN";
static int reconnect_retry_count = 0;
static const int MAX_RECONNECT_RETRIES = 10;

void wlan_task(void *parameter)
{
   while (!wlan_task_registered)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
    while (true)
    {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(1000));  
    }  
}

static void wlan_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data
)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        DEBUG_INFO(TAG, "WLAN Modul started - Connecting to AP...");
        reconnect_retry_count = 0;
        esp_wifi_connect();
        return;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_event_sta_disconnected_t *disconnected_data = (wifi_event_sta_disconnected_t *)event_data;
        uint8_t reason = (disconnected_data != NULL) ? disconnected_data->reason : 0;

        if (reconnect_retry_count < MAX_RECONNECT_RETRIES) {
            reconnect_retry_count++;
            DEBUG_WARN(TAG, "WLAN disconnected (Reason Code: %d). Retry %d/%d in 3 seconds...", 
                       reason, reconnect_retry_count, MAX_RECONNECT_RETRIES);
            
            vTaskDelay(pdMS_TO_TICKS(3000)); 
            esp_wifi_connect();
        } else {
            DEBUG_ERROR(TAG, "WLAN reconnect failed after %d retries. (Final Reason: %d)", 
                        MAX_RECONNECT_RETRIES, reason);
        }
        return;
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = static_cast<ip_event_got_ip_t *>(event_data);
        reconnect_retry_count = 0; 

        DEBUG_INFO(TAG, "WLAN connected - IP address: " IPSTR, IP2STR(&event->ip_info.ip));
        return;
    }
}

void wlan_init()
{
    DEBUG_INFO(TAG, "Starting WLAN initialization");

    if (!CONFIG_WLAN_ENABLED)
    {
        DEBUG_DEBUG(TAG, "WLAN Modul disabled - initialization skipped");
        return;
    }

    DEBUG_DEBUG(TAG, "Initializing NVS");
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        DEBUG_DEBUG(TAG, "Erasing NVS and reinitializing");
        result = nvs_flash_erase();
        if (result != ESP_OK)
        {
            DEBUG_ERROR(TAG, "NVS erase failed: %s", esp_err_to_name(result));
            return;
        }
        result = nvs_flash_init();
    }
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "NVS initialization failed: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_DEBUG(TAG, "Initializing network interface");
    result = esp_netif_init();
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE)
    {
        DEBUG_ERROR(TAG, "Network interface initialization failed: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_DEBUG(TAG, "Initializing default event loop");
    result = esp_event_loop_create_default();
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE)
    {
        DEBUG_ERROR(TAG, "Default event loop initialization failed: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_DEBUG(TAG, "Creating WLAN station interface");
    esp_netif_t *wifi_netif = esp_netif_create_default_wifi_sta();
    if (wifi_netif == nullptr)
    {
        DEBUG_ERROR(TAG, "Failed to create WLAN station interface");
        return;
    }

    DEBUG_DEBUG(TAG, "Initializing WLAN driver (Remote for ESP32-P4)");
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    
    result = esp_wifi_init(&wifi_init_config);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "WLAN driver initialization failed: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_DEBUG(TAG, "Configuring WLAN station");
    wifi_config_t wifi_config = {};
    
    // FRITZ!Box WPA2/WPA3 PMF-Aushandlung fixieren
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;
    
    // REINER 2,4 GHz SCAN-MODUS: Ignoriert die störenden 5 GHz Beacons der FRITZ!Box komplett
    wifi_config.sta.scan_method = WIFI_FAST_SCAN;
    wifi_config.sta.listen_interval = 3; 

    memset(wifi_config.sta.ssid, 0, sizeof(wifi_config.sta.ssid));
    memset(wifi_config.sta.password, 0, sizeof(wifi_config.sta.password));
    
    strncpy((char *)wifi_config.sta.ssid, CONFIG_WLAN_SSID, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char *)wifi_config.sta.password, CONFIG_WLAN_PASSWORD, sizeof(wifi_config.sta.password) - 1);

    DEBUG_INFO(TAG, "Target SSID: %s", (char *)wifi_config.sta.ssid);

    result = esp_wifi_set_mode(WIFI_MODE_STA);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to set WLAN station mode: %s", esp_err_to_name(result));
        return;
    }

    result = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to configure WLAN station: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_DEBUG(TAG, "Registering WLAN event handler");
    result = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wlan_event_handler, nullptr);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "WLAN event handler registration failed: %s", esp_err_to_name(result));
        return;
    }

    result = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wlan_event_handler, nullptr);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "IP event handler registration failed: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_INFO(TAG, "Starting WLAN");
    result = esp_wifi_start();
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "WLAN start failed: %s", esp_err_to_name(result));
        return;
    }

    DEBUG_INFO(TAG, "WLAN initialization completed");
}
