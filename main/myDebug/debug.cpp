#include "debug.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "myConfig/config.h"
#include "myWatchdog/watchdog_tasks.h"
#include <string.h>

static const char *TAG = "DEBUG-SYSTEM";

void debug_task(void *parameter)
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
        case ESP_LOG_ERROR:   return "ERROR";
        case ESP_LOG_WARN:    return "WARN";
        case ESP_LOG_INFO:    return "INFO";
        case ESP_LOG_DEBUG:   return "DEBUG";
        case ESP_LOG_VERBOSE: return "VERBOSE";
        default:              return "UNKNOWN";
    }
}
bool my_debug_should_log(const char* tag, esp_log_level_t level)
{
    if (!CONFIG_DEBUG_ENABLED) return false;

    if (tag != NULL) {
        if (strncmp(tag, "DEBUG-", 6) == 0) {
            return level <= CONFIG_DEBUG_LEVEL;
        }
    }

    return level <= ESP_LOG_INFO; 
}

void debug_init()
{
    if (!CONFIG_DEBUG_ENABLED)
    {
        esp_log_level_set("*", ESP_LOG_WARN);
        return;
    }
    
    esp_log_level_set("*", ESP_LOG_INFO);

    DEBUG_INFO(TAG, "Debug System initialized");
    DEBUG_INFO(TAG, "Debug Level for DEBUG-* Modules: %s", debug_level_name(CONFIG_DEBUG_LEVEL));
}
