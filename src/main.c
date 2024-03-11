/*

Copyright (c) 2017-2019 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.
By using this software you agree to the terms of the associated
Analog Devices Software License Agreement.

*/

#include <zephyr/kernel.h>
#include <ad5940.h>
#include <zephyr/shell/shell.h>

// /* Functions that used to initialize MCU platform */
// uint32_t MCUPlatformInit(void *pCfg);

// int main(void)
// {
//   extern void AD5940_Main(void);
//   MCUPlatformInit(NULL);
//   AD5940_MCUResourceInit(0);    /* Initialize resources that AD5940 use, like SPI/GPIO/Interrupt. */

//   printf("Hello AD5940-Build Time:%s\n",__TIME__);
//   AD5940_Main();
// }

// uint32_t MCUPlatformInit(void *pCfg)
// {
//   return 1;
// }





#include <zephyr/drivers/gpio.h>

void led_toggle_point(void *, void *, void *){
	int ret;

	ret = gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, GPIO_OUTPUT);
	if (ret < 0) {
		return;
	}

	while (1) {
		ret = gpio_pin_toggle(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12);
		if (ret < 0) {
			return;
		}
		k_msleep(500);
	}
}

K_THREAD_DEFINE(led_toggle_tid, 512,
                led_toggle_point, NULL, NULL, NULL,
                10, 0, 0);


int oled_power_on(void) {
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, GPIO_OUTPUT);
	gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, 1);
	return 0;
}

SYS_INIT(oled_power_on, EARLY, 0);

/*
 * Copyright (c) 2018 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

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
LOG_MODULE_REGISTER(app);

static uint32_t count;

#ifdef CONFIG_GPIO
static struct gpio_dt_spec button_gpio = GPIO_DT_SPEC_GET_OR(
		DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_callback;

static void button_isr_callback(const struct device *port,
				struct gpio_callback *cb,
				uint32_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	count = 0;
}
#endif /* CONFIG_GPIO */

#ifdef CONFIG_LV_Z_ENCODER_INPUT
static const struct device *lvgl_encoder =
	DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_encoder_input));
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

static void lv_btn_click_callback(lv_event_t *e)
{
	ARG_UNUSED(e);

	count = 0;
}

int main(void)
{
	char count_str[11] = {0};
	const struct device *display_dev;
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

	LOG_INF("init? mono?? %d", lv_theme_mono_is_inited());

    /* 사용할 폰트 지정. 예: lv_font_montserrat_14 */
    const lv_font_t * font = &lv_font_montserrat_14;

    /* mono 테마 설정, 폰트 인자 추가 */
    lv_theme_t * th = lv_theme_mono_init(NULL, true, font);
    lv_disp_set_theme(lv_disp_get_default(), th);

#ifdef CONFIG_GPIO
	if (gpio_is_ready_dt(&button_gpio)) {
		int err;

		err = gpio_pin_configure_dt(&button_gpio, GPIO_INPUT);
		if (err) {
			LOG_ERR("failed to configure button gpio: %d", err);
			return 0;
		}

		gpio_init_callback(&button_callback, button_isr_callback,
				   BIT(button_gpio.pin));

		err = gpio_add_callback(button_gpio.port, &button_callback);
		if (err) {
			LOG_ERR("failed to add button callback: %d", err);
			return 0;
		}

		err = gpio_pin_interrupt_configure_dt(&button_gpio,
						      GPIO_INT_EDGE_TO_ACTIVE);
		if (err) {
			LOG_ERR("failed to enable button callback: %d", err);
			return 0;
		}
	}
#endif /* CONFIG_GPIO */

#ifdef CONFIG_LV_Z_ENCODER_INPUT
	lv_obj_t *arc;
	lv_group_t *arc_group;

	arc = lv_arc_create(lv_scr_act());
	lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_size(arc, 150, 150);

	arc_group = lv_group_create();
	lv_group_add_obj(arc_group, arc);
	lv_indev_set_group(lvgl_input_get_indev(lvgl_encoder), arc_group);
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN) || IS_ENABLED(CONFIG_LV_Z_POINTER_INPUT)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		lv_obj_add_event_cb(hello_world_button, lv_btn_click_callback, LV_EVENT_CLICKED,
						NULL);
		hello_world_label = lv_label_create(hello_world_button);
	} else {
		hello_world_label = lv_label_create(lv_scr_act());
	}

	lv_label_set_text(hello_world_label, "Hello world!");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	lv_task_handler();
	display_blanking_off(display_dev);

	int toggle = 0;
	while (1) {
		if ((count % 100) == 0U) {
			if (toggle%2) {
				display_blanking_on(display_dev);
				LOG_INF("blanking on");
			} else {
				display_blanking_off(display_dev);
				LOG_INF("blanking off");
			}
			toggle += 1;
			sprintf(count_str, "%d", count/100U);
			lv_label_set_text(count_label, count_str);
			// gpio_pin_toggle(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17);
		}

		lv_task_handler();
		++count;
		k_sleep(K_MSEC(10));
	}
}