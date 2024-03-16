#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/input/input.h>

#include <ad5940.h>

/* Functions that used to initialize MCU platform */
uint32_t MCUPlatformInit(void *pCfg);

static void ad5940_main(void *, void *, void *)
{
  extern void AD5940_Main(void);
  MCUPlatformInit(NULL);
  AD5940_MCUResourceInit(0);
  AD5940_Main();
}

uint32_t MCUPlatformInit(void *pCfg)
{
  k_msleep(1000);
  return 1;
}

K_THREAD_DEFINE(ad5940_tid, 2048,
                ad5940_main, NULL, NULL, NULL,
                4, 0, 0);