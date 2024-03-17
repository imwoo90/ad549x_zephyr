#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/input/input.h>

#include <ad5940.h>

K_THREAD_STACK_DEFINE(ad5940_main_stack, 4096);
struct k_thread ad5940_main_tid;

/* Functions that used to initialize MCU platform */
uint32_t MCUPlatformInit(void *pCfg);

void ad5940_main(void *, void *, void *)
{
  extern void AD5940_Main(void);
  MCUPlatformInit(NULL);
  AD5940_MCUResourceInit(0);
  AD5940_Main();
}

void create_ad5940_main(void) {
  k_thread_create(&ad5940_main_tid, ad5940_main_stack, K_THREAD_STACK_SIZEOF(ad5940_main_stack), 
          ad5940_main, NULL, NULL, NULL, 4, 0, K_NO_WAIT);
}

uint32_t MCUPlatformInit(void *pCfg)
{
	// sensor power on
	gpio_pin_configure(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 19, GPIO_OUTPUT);
  gpio_pin_set(DEVICE_DT_GET(DT_NODELABEL(gpio0)), 19, 1);
  k_msleep(1000);
  return 1;
}
