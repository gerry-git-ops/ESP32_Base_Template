#pragma once

/*===========================================================
ESP32 Base Template - Configuration
===========================================================*/

/*===========================================================
Serial Debugging
===========================================================*/

#define CONFIG_DEBUG_ENABLED true

//#define CONFIG_DEBUG_LEVEL ESP_LOG_INFO
// #define CONFIG_DEBUG_LEVEL ESP_LOG_ERROR
// #define CONFIG_DEBUG_LEVEL ESP_LOG_WARN
// #define CONFIG_DEBUG_LEVEL ESP_LOG_DEBUG
#define CONFIG_DEBUG_LEVEL ESP_LOG_VERBOSE

/*===========================================================
Hardware Watchdog
===========================================================*/

#define CONFIG_WATCHDOG_ENABLED true
#define CONFIG_WATCHDOG_TIMEOUT_MS 5000


/*===========================================================
WiFi Configuration
===========================================================*/

#define CONFIG_WLAN_ENABLES true
#define CONFIG_WLAN_SSID "YOUcoolSSID"
#define CONFIG_WLAN_PASSWORD "YOUcoolPASSWORD" 