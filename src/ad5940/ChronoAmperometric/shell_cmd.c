#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "ChronoAmperometric.h"

extern uint32_t AppBuff[_n][APPBUFF_SIZE];
static int run_chrono_amperometric_cmd(const struct shell* shell, size_t argc, char **argv)
{
	AppCHRONOAMPInit(AppBuff[0], APPBUFF_SIZE);    /* Initialize AMP application. Provide a buffer, which is used to store sequencer commands */
	AppCHRONOAMPCtrl(CHRONOAMPCTRL_PULSETEST, 0);         /* Control AMP measurement. AMPCTRL_PULSETEST carries out pulse test*/
	return 0;
}

SHELL_CMD_REGISTER(run_chrono_amperometric, NULL, "run Chrono Amperometric", run_chrono_amperometric_cmd);