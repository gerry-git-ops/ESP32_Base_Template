#include "debug.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "myConfig/config.h"
#include "myWatchdog/watchdog_tasks.h"

static const char *TAG = "DEBUG-SYSTEM";

extern void debug_task(void *parameter)
{
    while (!debug_task_registered)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
    while (true)
    {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(1000));  
    }  
}

static const char *debug_level_name(esp_log_level_t level)
{
    switch (level)
    {
        case ESP_LOG_ERROR:
            return "ERROR";
        
        case ESP_LOG_WARN:
            return "WARN";

        case ESP_LOG_INFO:
            return "INFO";

        case ESP_LOG_DEBUG:
            return "DEBUG";

        case ESP_LOG_VERBOSE:
            return "VERBOSE";

        default:
            return "UNKNOWN";
    }
}

void debug_init()
{
    ESP_LOGI(TAG, "Initializing Debug System....");
    if (!CONFIG_DEBUG_ENABLED)
    {
        ESP_LOGI(TAG, "Debug System not enabled");
        return;
    }
    
    ESP_LOGI(TAG, "Configuring Debug System....");
    esp_log_level_set("*", CONFIG_DEBUG_LEVEL);

    DEBUG_INFO(TAG, "Debug System initialized");
    DEBUG_INFO(TAG, "Debug Level: %s", debug_level_name(CONFIG_DEBUG_LEVEL));
}