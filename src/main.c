#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

extern void power_off();

int main(void)
{
	// extern void create_ad5940_main(void);
	// create_ad5940_main();
	extern void ad5940_main(void *, void *, void *);
	ad5940_main(NULL, NULL, NULL);
	return 0;
}


#include <zephyr/drivers/gpio.h>

void led_toggle_point(void *, void *, void *){
	int ret;

	ret = gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, GPIO_OUTPUT);
	if (ret < 0) {
		return;
	}

	int cnt = 0;
	while (1) {
		if (cnt == 5) {
			ret = gpio_pin_toggle(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12);
			if (ret < 0) {
				return;
			}
			cnt = 0;
		}
		if (gpio_pin_get(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 30) == 1) {
			power_off();
			return;
		}
		cnt+=1;
		k_msleep(100);
	}
}

K_THREAD_DEFINE(led_toggle_tid, 512,
                led_toggle_point, NULL, NULL, NULL,
                14, 0, 0);

int module_power_init(void) {
	// power_off pin
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 28, GPIO_OUTPUT);
	gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 28, 1);

	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 30, GPIO_INPUT);
	if (gpio_pin_get(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 30) == 1) {
		power_off();
	}

	// led on
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, GPIO_OUTPUT);
	gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, 1);

	// display power on
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, GPIO_OUTPUT);
	gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, 1);
	return 0;
}

SYS_INIT(module_power_init, EARLY, 0);
