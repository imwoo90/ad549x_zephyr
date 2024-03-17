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

#include "battery_status.ui.h"

typedef struct _display_data {
    uint32_t battery_pptt;
    double mid_data; //mid data of mesurement uA
    double last_data; //last data of mesurement uA
} display_data;



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

	while (1) {
		lv_task_handler();
		k_msleep(50);
	}
}


K_THREAD_DEFINE(ui_tid, 4096,
                ui_main, NULL, NULL, NULL,
                14, 0, 0);