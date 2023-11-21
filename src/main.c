/*

Copyright (c) 2017-2019 Analog Devices, Inc. All Rights Reserved.

This software is proprietary to Analog Devices, Inc. and its licensors.
By using this software you agree to the terms of the associated
Analog Devices Software License Agreement.

*/

#include <zephyr/kernel.h>
#include <ad5940.h>

/* Functions that used to initialize MCU platform */
uint32_t MCUPlatformInit(void *pCfg);

int main(void)
{
  extern void AD5940_Main(void);
  MCUPlatformInit(NULL);
  AD5940_MCUResourceInit(0);    /* Initialize resources that AD5940 use, like SPI/GPIO/Interrupt. */

  printf("Hello AD5940-Build Time:%s\n",__TIME__);
  AD5940_Main();
}

uint32_t MCUPlatformInit(void *pCfg)
{
  return 1;
}





#include <zephyr/drivers/gpio.h>

void led_toggle_point(void *, void *, void *){
	int ret;

	ret = gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 31, GPIO_OUTPUT);
	if (ret < 0) {
		return;
	}

	while (1) {
		ret = gpio_pin_toggle(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 31);
		if (ret < 0) {
			return;
		}
		k_msleep(500);
	}
}

K_THREAD_DEFINE(led_toggle_tid, 512,
                led_toggle_point, NULL, NULL, NULL,
                10, 0, 0);



