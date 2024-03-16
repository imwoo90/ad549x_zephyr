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










static void ui_main(void *, void *, void *) {
	char count_str[11] = {0};
	const struct device *display_dev;
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return ;
	}

    /* 사용할 폰트 지정. 예: lv_font_montserrat_14 */
    const lv_font_t * font = &lv_font_montserrat_14;

    /* mono 테마 설정, 폰트 인자 추가 */
    lv_theme_t * th = lv_theme_mono_init(NULL, true, font);
    lv_disp_set_theme(lv_disp_get_default(), th);

    // lv_obj_t * label1 = lv_label_create(lv_scr_act());
    // lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    // lv_label_set_text(label1, "Recolor is not supported for v9 now.");
    // lv_obj_set_width(label1, 120);  /*Set smaller width to make the lines wrap*/
    // lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    // lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label2, 120);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);

	int toggle = 0;
	while (1) {
		lv_task_handler();
		k_msleep(50);
	}
}


K_THREAD_DEFINE(ui_tid, 2048,
                ui_main, NULL, NULL, NULL,
                14, 0, 0);