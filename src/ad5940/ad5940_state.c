#include <ad5940_state.h>
#include <ui.h>

static int g_is_busy = AD5940_STATE_BUSY;
int get_ad5940_state(void) {
	return g_is_busy;
}

int set_ad5940_state(int state) {
	g_is_busy = state;
    update_ad5940_state(state);
}