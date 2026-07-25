#include <stdio.h>
#include "myConfig/config.h"
#include "myDebug/debug.h"

static const char *TAG = "DEBUG-MAIN";

extern "C" void app_main(void)
{
    debug_init();
    DEBUG_INFO(TAG, "Main Started");
}
