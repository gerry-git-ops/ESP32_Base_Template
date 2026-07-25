#include "watchdog.h"
#include "myDebug/debug.h"
#include "myConfig/config.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DEBUG-WATCHDOG";
static TaskHandle_t watchdog_task_handle = nullptr;
static bool watchdog_task_registered = false;

static void watchdog_task(void *parameter)
{
    while (!watchdog_task_registered)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
    while (true)
    {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(1000));  
    }  
}

void watchdog_init()
{
    DEBUG_DEBUG(TAG, "Initializing Watchdog System...." );

    if (!CONFIG_WATCHDOG_ENABLED)
    {
        DEBUG_DEBUG(TAG, "Watchdog System not Enabled");
        return;
    }

    esp_task_wdt_config_t config = {
        .timeout_ms = CONFIG_WATCHDOG_TIMEOUT_MS,
        .idle_core_mask = 0,
        .trigger_panic = true
    };

    DEBUG_DEBUG(TAG, "Configuring Watchdog System...." );
    
    esp_err_t result = esp_task_wdt_init(&config);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Watchdog initialization failed: %s", esp_err_to_name(result));
        return;
    }
    esp_task_wdt_add(NULL);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to add app_main to Watchdog: %s", esp_err_to_name(result));
        return;
    }
    xTaskCreate(
        watchdog_task,
        "watchdog_task",
        2048,
        nullptr,
        5,
        &watchdog_task_handle
    );
    esp_task_wdt_add(watchdog_task_handle);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to add Watchdog Task to Watchdog: %s", esp_err_to_name(result));
        return;
    }
    watchdog_task_registered = true;

    DEBUG_INFO(TAG, "Watchdog System initialized");
}