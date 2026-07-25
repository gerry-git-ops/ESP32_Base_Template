#include <stdio.h>
#include "myConfig/config.h"
#include "myDebug/debug.h"

extern "C" void app_main(void)
{
    debug_init();
    printf("Hello world!\n");
}
