#include <stdio.h>
#include "myDebug/debug.h"
#include "myWatchdog/watchdog.h"

static const char *TAG = "DEBUG-MAIN";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Main started");
    debug_init();
    watchdog_init();
}
