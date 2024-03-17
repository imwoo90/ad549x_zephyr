#pragma once

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>


typedef struct {
    lv_obj_t *container;    // 각 데이터 라인을 포함할 컨테이너
    lv_obj_t *label_title;  // "MID" 또는 "LAST"
    lv_obj_t *label_value;  // 데이터 값
    lv_obj_t *label_unit;   // "uA"
} DataLine;


DataLine create_data_line(lv_obj_t* parent, const char* title, int y_offset);
void update_data_line(DataLine* line, float value);