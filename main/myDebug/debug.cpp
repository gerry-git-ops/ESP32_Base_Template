#include "debug.h"
#include "esp_log.h"
#include "myConfig/config.h"

static const char *TAG = "DEBUG-SYSTEM";

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