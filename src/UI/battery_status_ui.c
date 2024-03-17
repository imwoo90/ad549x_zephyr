#include "battery_status.ui.h"

typedef struct {
    lv_obj_t *container;
    lv_obj_t *charging_label;
    lv_obj_t *battery_label;
    lv_obj_t *percentage_label;
} BatteryStatusUI;

static BatteryStatusUI g_battery_status_ui;

void create_battery_status_ui(lv_obj_t* parent) {
    BatteryStatusUI* ui = &g_battery_status_ui;

    ui->container = lv_obj_create(parent);
    // 스크롤을 무시하도록 설정
    lv_obj_clear_flag(ui->container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_width(ui->container, 128); // 컨테이너의 너비를 200px로 설정
    lv_obj_set_height(ui->container, 24); // 컨테이너의 높이를 50px로 설정
    lv_obj_align(ui->container, LV_ALIGN_TOP_LEFT, 0, 0);

    ui->charging_label = lv_label_create(ui->container);
    lv_label_set_text(ui->charging_label, LV_SYMBOL_CHARGE);
    lv_obj_set_style_text_font(ui->charging_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(ui->charging_label, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_add_flag(ui->charging_label, LV_OBJ_FLAG_HIDDEN); // Initially hidden

    ui->battery_label = lv_label_create(ui->container);
    lv_label_set_text(ui->battery_label, LV_SYMBOL_BATTERY_EMPTY); // Initial symbol
    lv_obj_set_style_text_font(ui->battery_label, &lv_font_montserrat_24, 0);
    lv_obj_align_to(ui->battery_label, ui->charging_label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    ui->percentage_label = lv_label_create(ui->container);
    lv_label_set_text(ui->percentage_label, "0%"); // Initial text
    lv_obj_set_style_text_font(ui->percentage_label, &lv_font_montserrat_16, 0);
    lv_obj_align(ui->percentage_label, LV_ALIGN_RIGHT_MID, 0, -3);
}

void update_battery_status_ui(int battery_level, bool is_charging) {
    BatteryStatusUI* ui = &g_battery_status_ui;
    if (is_charging) {
        lv_obj_clear_flag(ui->charging_label, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(ui->charging_label, LV_OBJ_FLAG_HIDDEN);
    }

    const char* battery_symbol = battery_level > 75 ? LV_SYMBOL_BATTERY_FULL :
                                 battery_level > 50 ? LV_SYMBOL_BATTERY_3 :
                                 battery_level > 25 ? LV_SYMBOL_BATTERY_2 :
                                 battery_level > 10 ? LV_SYMBOL_BATTERY_1 :
                                 LV_SYMBOL_BATTERY_EMPTY;
    lv_label_set_text(ui->battery_label, battery_symbol);

    char percentage_text[10];
    sprintf(percentage_text, "%d%%", battery_level);
    lv_label_set_text(ui->percentage_label, percentage_text);
}
