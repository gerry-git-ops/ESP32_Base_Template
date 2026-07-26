#pragma once

#include "esp_log.h"

extern void debug_task(void *parameter);

void debug_init();

#define DEBUG_ERROR(TAG, ...)   ESP_LOGE(TAG, __VA_ARGS__)
#define DEBUG_WARN(TAG, ...)    ESP_LOGW(TAG, __VA_ARGS__)
#define DEBUG_INFO(TAG, ...)    ESP_LOGI(TAG, __VA_ARGS__)
#define DEBUG_DEBUG(TAG, ...)   ESP_LOGD(TAG, __VA_ARGS__)
#define DEBUG_VERBOSE(TAG, ...) ESP_LOGV(TAG, __VA_ARGS__)