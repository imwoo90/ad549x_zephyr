#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);
int main(void)
{
	while(1) {
		LOG_INF("charge in [%d]", gpio_pin_get(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 30));
		k_msleep(10000);
	}
	return 0;
}


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

int module_power_init(void) {
	// led on
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, GPIO_OUTPUT);
	gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, 1);

	// display power on
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, GPIO_OUTPUT);
	gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, 1);
	return 0;
}

SYS_INIT(module_power_init, EARLY, 0);
