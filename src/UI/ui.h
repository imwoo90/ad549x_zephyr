#pragma once

#include <zephyr/kernel.h>

void update_battery_status(int battery_level, bool is_charging);
void update_sensor_data(float m, float l);