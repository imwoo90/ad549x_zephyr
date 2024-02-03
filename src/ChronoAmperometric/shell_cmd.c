#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "SqrWaveVoltammetry.h"

extern uint32_t AppBuff[3][APPBUFF_SIZE];
static int run_sqr_wave_cmd(const struct shell* shell, size_t argc, char **argv)
{
	AppCHRONOAMPInit(AppBuff[0], APPBUFF_SIZE);    /* Initialize AMP application. Provide a buffer, which is used to store sequencer commands */
	AppCHRONOAMPCtrl(CHRONOAMPCTRL_PULSETEST, 0);         /* Control AMP measurement. AMPCTRL_PULSETEST carries out pulse test*/
}

SHELL_CMD_REGISTER(run_sqr_wave, NULL, "run Chrono Amperometric", run_sqr_wave_cmd);