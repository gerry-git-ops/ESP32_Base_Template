#include <stdio.h>
#include "myDebug/debug.h"
#include "myWatchdog/watchdog.h"
#include "myWLAN/wlan.h"
#include "esp_task_wdt.h"

static const char *TAG = "DEBUG-MAIN";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Main started");
    debug_init();
    watchdog_init();
    wlan_init();
    
    
     while (true)
    {
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(1000));  
    }
}
