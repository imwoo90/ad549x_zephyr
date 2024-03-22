#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(UI);

#include "ui.h"
#include "battery_status.ui.h"
#include "mesurement_ui.h"

K_MUTEX_DEFINE(lvgl_mutex);

DataLine mid;
DataLine last;

void update_sensor_data(float m, float l) {
	if(last.label_value == NULL)
		return;
	
	k_mutex_lock(&lvgl_mutex, K_FOREVER);
	update_data_line(&mid, m);
	update_data_line(&last, l);
	k_mutex_unlock(&lvgl_mutex);
}

void update_battery_status(int battery_level, bool is_charging) {
	k_mutex_lock(&lvgl_mutex, K_FOREVER);
	update_battery_status_ui(battery_level, is_charging);
	k_mutex_unlock(&lvgl_mutex);
}

static void ui_main(void *, void *, void *) {
	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return ;
	}

    /* 사용할 폰트 지정. 예: lv_font_montserrat_14 */
    const lv_font_t * font = &lv_font_montserrat_24;

    /* mono 테마 설정, 폰트 인자 추가 */
    lv_theme_t * th = lv_theme_mono_init(NULL, true, font);
    lv_disp_set_theme(lv_disp_get_default(), th);

    create_battery_status_ui(lv_scr_act());
    update_battery_status_ui(30, 1);

	mid = create_data_line(lv_scr_act(), "M", 24);
	last = create_data_line(lv_scr_act(), "L", 44);

	update_sensor_data(-0.138948, -0.104211);

	while (1) {
		k_mutex_lock(&lvgl_mutex, K_FOREVER);
		lv_task_handler();
		k_mutex_unlock(&lvgl_mutex);
		k_msleep(50);
	}
}


K_THREAD_DEFINE(ui_tid, 4096,
                ui_main, NULL, NULL, NULL,
                14, 0, 0);