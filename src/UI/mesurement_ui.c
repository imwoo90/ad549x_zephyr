#include "battery_status.ui.h"
#include "mesurement_ui.h"

DataLine create_data_line(lv_obj_t* parent, const char* title, int y_offset) {
    DataLine line;

    // 컨테이너 생성 및 크기 설정
    line.container = lv_obj_create(parent);
	// 스크롤을 무시하도록 설정
    lv_obj_clear_flag(line.container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(line.container, 128, 20);
    lv_obj_align(line.container, LV_ALIGN_TOP_LEFT, 0, y_offset);

    // "MID" 또는 "LAST" 라벨
    line.label_title = lv_label_create(line.container);
    lv_label_set_text(line.label_title, title);
    lv_obj_set_style_text_font(line.label_title, &lv_font_montserrat_16, 0);
    lv_obj_align(line.label_title, LV_ALIGN_LEFT_MID, 0, 0);

    // 데이터 값 라벨
    line.label_value = lv_label_create(line.container);
    lv_label_set_text(line.label_value, "0 uA");
    lv_obj_set_style_text_font(line.label_value, &lv_font_montserrat_16, 0);
    lv_obj_align(line.label_value, LV_ALIGN_RIGHT_MID, 0, 0);

    return line;
}

void update_data_line(DataLine* line, float value) {
    lv_label_set_text_fmt(line->label_value, "%f uA", value);

    // // "uA" 단위 라벨을 컨테이너의 오른쪽 끝에 정렬
    // lv_obj_align(line->label_unit, LV_ALIGN_RIGHT_MID, 0, 0);

    // // 데이터 값 라벨을 "uA" 단위 라벨의 왼쪽에 정렬
    // lv_obj_align_to(line->label_value, line->label_unit, LV_ALIGN_OUT_LEFT_MID, -5, 0);
}