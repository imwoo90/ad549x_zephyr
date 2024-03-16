#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>

#include <zephyr/pm/device.h>
#include <zephyr/sys/poweroff.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ISR);

static void power_off(struct input_event *evt)  {
    // int rc = pm_device_action_run(cons, PM_DEVICE_ACTION_SUSPEND);
    // if (rc < 0) {
    // 	printf("Could not suspend console (%d)\n", rc);
    // 	return 0;
    // }
    gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, GPIO_INPUT);
    gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, GPIO_INPUT);
    gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 19, GPIO_INPUT);

    gpio_pin_interrupt_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 29, GPIO_INT_LEVEL_ACTIVE);
    gpio_pin_interrupt_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 30, GPIO_INT_LEVEL_ACTIVE);

    sys_poweroff();
}

static void action_logical_low(struct input_event *evt) {
    switch (evt->code)
    {
    case INPUT_KEY_0:
        break;
    case INPUT_KEY_1:
    case INPUT_KEY_2:
        break;
    }
}

static void action_logical_high(struct input_event *evt) {
    switch (evt->code)
    {
    case INPUT_KEY_0:
        power_off(evt);
        break;
    case INPUT_KEY_1:
    case INPUT_KEY_2:
        break;
    }
}

static void gpio_intterupt_cb(struct input_event *evt)
{
	if (evt->type != INPUT_EV_KEY)
		return ;
	if (evt->value) {
        action_logical_high(evt);
	} else { //release
        action_logical_low(evt);
	}
	LOG_INF("test type[%d] value[%d] code[%d] sync[%d]", evt->type, evt->value, evt->code, evt->sync);
	return;
}
INPUT_CALLBACK_DEFINE(NULL, gpio_intterupt_cb);