#pragma once

enum {
    AD5940_STATE_READY,
    AD5940_STATE_BUSY,
};

int get_ad5940_state(void);
int set_ad5940_state(int state);