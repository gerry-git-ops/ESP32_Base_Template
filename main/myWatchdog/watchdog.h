#pragma once

extern bool  watchdog_task_registered;

extern void watchdog_task(void *parameter);

void watchdog_init();