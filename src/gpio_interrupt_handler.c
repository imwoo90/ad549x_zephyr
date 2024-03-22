#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>

#include <zephyr/pm/device.h>
#include <zephyr/sys/poweroff.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ISR);

#include <battery.h>
#include <ad5940_state.h>

K_SEM_DEFINE(sensor_work, 0, 1);

static void isr_handler(void *, void *, void *) {

    while(1) {
        k_sem_take(&sensor_work, K_FOREVER);
        shell_execute_cmd(NULL, "run_sqr_wave");
        shell_execute_cmd(NULL, "run_chrono_amperometric");
    }
}
K_THREAD_DEFINE(isr_handler_tid, 4096,
                isr_handler, NULL, NULL, NULL,
                14, 0, 0);

void power_off()  {
    // int rc = pm_device_action_run(cons, PM_DEVICE_ACTION_SUSPEND);
    // if (rc < 0) {
    // 	printf("Could not suspend console (%d)\n", rc);
    // 	return 0;
    // }
    gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 12, GPIO_INPUT);
    gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 17, GPIO_INPUT);
    gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 19, GPIO_INPUT);

    gpio_pin_interrupt_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 29, GPIO_INT_LEVEL_ACTIVE);
    // gpio_pin_interrupt_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 30, GPIO_INT_EDGE_RISING);

    sys_poweroff();
}

static void action_logical_low(struct input_event *evt) {
    switch (evt->code)
    {
    case INPUT_KEY_0:
        break;
    case INPUT_KEY_1:
        break;
    case INPUT_KEY_2:
        set_charging_status(true);
        break;
    case INPUT_KEY_3:
        break;
    }
}

static void action_logical_high(struct input_event *evt) {
    switch (evt->code)
    {
    case INPUT_KEY_0:
        power_off();
        break;
    case INPUT_KEY_1:
        power_off();
        break;
    case INPUT_KEY_2:
        set_charging_status(false);
        break;
    case INPUT_KEY_3:
        if (get_ad5940_state() == AD5940_STATE_BUSY)
            return;
        k_sem_give(&sensor_work);
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