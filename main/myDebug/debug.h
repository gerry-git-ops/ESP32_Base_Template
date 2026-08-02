#pragma once

#include "esp_log.h"
#include <stdbool.h>

extern void debug_task(void *parameter);
void debug_init();

bool my_debug_should_log(const char* tag, esp_log_level_t level);

#define DEBUG_ERROR(TAG, ...)   if(my_debug_should_log(TAG, ESP_LOG_ERROR))   ESP_LOGE(TAG, __VA_ARGS__)
#define DEBUG_WARN(TAG, ...)    if(my_debug_should_log(TAG, ESP_LOG_WARN))    ESP_LOGW(TAG, __VA_ARGS__)
#define DEBUG_INFO(TAG, ...)    if(my_debug_should_log(TAG, ESP_LOG_INFO))    ESP_LOGI(TAG, __VA_ARGS__)
#define DEBUG_DEBUG(TAG, ...)   if(my_debug_should_log(TAG, ESP_LOG_DEBUG))   ESP_LOGD(TAG, __VA_ARGS__)
#define DEBUG_VERBOSE(TAG, ...) if(my_debug_should_log(TAG, ESP_LOG_VERBOSE)) ESP_LOGV(TAG, __VA_ARGS__)
