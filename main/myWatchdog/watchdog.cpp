#include "watchdog.h"
#include "watchdog_tasks.h"
#include "myDebug/debug.h"
#include "myConfig/config.h"
#include "esp_task_wdt.h"

static const char *TAG = "DEBUG-WATCHDOG";

extern void watchdog_task(void *parameter)
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
    DEBUG_INFO(TAG, "Watchdog System initialized");

    watchdog_tasks_init();   
}