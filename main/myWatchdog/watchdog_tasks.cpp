#include "watchdog_tasks.h"
#include "watchdog.h"
#include "myDebug/debug.h"
#include "myConfig/config.h"
#include "esp_task_wdt.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"

static const char *TAG = "DEBUG-WATCHDOG-TASK";

static TaskHandle_t watchdog_task_handle = nullptr;
static TaskHandle_t debug_task_handle = nullptr;
//static TaskHandle_t YURcoolMODUL_task_handle = nullptr;

extern bool debug_task_registered = false;

 /*===========================================================
    app-main
    ===========================================================*/
void main_task()
{
    DEBUG_DEBUG(TAG, "Adding app_main Task to Watchdog System...." );
    esp_err_t result = esp_task_wdt_add(NULL);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to add app_main to Watchdog: %s", esp_err_to_name(result));
        return;
    }
    DEBUG_INFO(TAG, "Adding app_main Task to Watchdog System successful" );
}

 /*===========================================================
    Watchdog-Modul
    ===========================================================*/
void wachdog_task()
{
    DEBUG_DEBUG(TAG, "Adding Watchdog Task to Watchdog System...." );
    xTaskCreate(
        watchdog_task,
        "watchdog_task",
        2048,
        nullptr,
        5,
        &watchdog_task_handle
    );
    esp_err_t result = esp_task_wdt_add(watchdog_task_handle);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to add Watchdog Task to Watchdog: %s", esp_err_to_name(result));
        return;
    }
    watchdog_task_registered = true;
    DEBUG_INFO(TAG, "Adding Watchdog Task to Watchdog System successful" );
}

/*===========================================================
    Debug-Modul
    ===========================================================*/
void debug_task()
{
    if (!CONFIG_DEBUG_ENABLED)
    {
        DEBUG_DEBUG(TAG, "Debug Modul not Enabled");
        return;
    }
    DEBUG_DEBUG(TAG, "Adding Debug Task to Watchdog System...." );
    xTaskCreate(
        debug_task,
        "debug_task",
        2048,
        nullptr,
        5,
        &debug_task_handle
    );
    esp_err_t result = esp_task_wdt_add(debug_task_handle);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to add Debug Task to Watchdog: %s", esp_err_to_name(result));
        return;
    }
    debug_task_registered = true;
    DEBUG_INFO(TAG, "Adding Debug Task to Watchdog System successful" );
}

/*===========================================================
    YOUcoolMODUL
    ===========================================================*/
/*void YOUcoolMODUL_task()
{
    if (!CONFIG_YOUcoolMODUL_ENABLED)
    {
        DEBUG_DEBUG(TAG, "YOUcoolMODUL Modul not Enabled");
        return;
    }
    DEBUG_DEBUG(TAG, "Adding YOUcoolMODUL Task to Watchdog System...." );
    xTaskCreate(
        YOUcoolMODUL_task,
        "YOUcoolMODUL_task",
        2048,
        nullptr,
        5,
        &YOUcoolMODUL_task_handle
    );
    esp_err_t result = esp_task_wdt_add(YOUcoolMODUL_task_handle);
    if (result != ESP_OK)
    {
        DEBUG_ERROR(TAG, "Failed to add YOUcoolMODUL Task to Watchdog: %s", esp_err_to_name(result));
        return;
    }
    DEBUG_INFO(TAG, "Adding YOUcoolMODUL Task to Watchdog System successful" );
}*/

void watchdog_tasks_init()
{
    main_task();
    wachdog_task();
    debug_task();
    //YOUcoolMODUL_task();  
}
