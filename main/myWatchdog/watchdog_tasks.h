#pragma once

extern bool debug_task_registered;
extern bool wlan_task_registered;
extern bool watchdog_task_registered;

void watchdog_tasks_init();