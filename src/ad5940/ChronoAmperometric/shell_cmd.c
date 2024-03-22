#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "ChronoAmperometric.h"
#include <ad5940_state.h>

extern void AD5940AMPStructInit(void);
extern uint32_t AppBuff[_n][APPBUFF_SIZE];
static int run_chrono_amperometric_cmd(const struct shell* shell, size_t argc, char **argv)
{
	set_ad5940_state(AD5940_STATE_BUSY);
	printf("here1\n");
	AD5940AMPStructInit(); /* Configure your parameters in this function */
	printf("here2\n");
	AppCHRONOAMPInit(AppBuff[0], APPBUFF_SIZE);    /* Initialize AMP application. Provide a buffer, which is used to store sequencer commands */
	printf("here3\n");
	AppCHRONOAMPCtrl(CHRONOAMPCTRL_PULSETEST, 0);         /* Control AMP measurement. AMPCTRL_PULSETEST carries out pulse test*/
	printf("here4\n");
	return 0;
}

SHELL_CMD_REGISTER(run_chrono_amperometric, NULL, "run Chrono Amperometric", run_chrono_amperometric_cmd);