#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "SqrWaveVoltammetry.h"

#define APPBUFF_SIZE 1024
extern uint32_t AppBuff[APPBUFF_SIZE];
static int run_sqr_wave_cmd(const struct shell* shell, size_t argc, char **argv)
{
	// AD5940PlatformCfg();
	AD5940RampStructInit();

	//AD5940_McuSetLow();
	AppSWVInit(AppBuff, APPBUFF_SIZE);    /* Initialize RAMP application. Provide a buffer, which is used to store sequencer commands */


	AD5940_Delay10us(100000);		/* Add a delay to allow sensor reach equilibrium befor starting the measurement */
	AppSWVCtrl(APPCTRL_START, 0);          /* Control IMP measurement to start. Second parameter has no meaning with this command. */
}

SHELL_CMD_REGISTER(run_sqr_wave, NULL, "run square wave", run_sqr_wave_cmd);