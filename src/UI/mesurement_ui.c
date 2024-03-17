#include "battery_status.ui.h"

typedef struct {
    lv_obj_t *container;    // 각 데이터 라인을 포함할 컨테이너
    lv_obj_t *label_title;  // "MID" 또는 "LAST"
    lv_obj_t *label_value;  // 데이터 값
    lv_obj_t *label_unit;   // "uA"
} DataLine;


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

    // "uA" 단위 라벨
    line.label_unit = lv_label_create(line.container);
    lv_label_set_text(line.label_unit, "uA");
    lv_obj_set_style_text_font(line.label_unit, &lv_font_montserrat_16, 0);
    lv_obj_align(line.label_unit, LV_ALIGN_RIGHT_MID, 0, 0);

    // 데이터 값 라벨
    line.label_value = lv_label_create(line.container);
    lv_label_set_text(line.label_value, "0.000");
    lv_obj_set_style_text_font(line.label_value, &lv_font_montserrat_16, 0);
    // 데이터 값 라벨을 "uA" 단위 라벨의 왼쪽에 정렬
    lv_obj_align_to(line.label_value, line.label_unit, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    return line;
}

void update_data_line(DataLine* line, float value) {
    char buf[16];
    sprintf(buf, "%.3f", value);
    lv_label_set_text(line->label_value, buf);

    // "uA" 단위 라벨을 컨테이너의 오른쪽 끝에 정렬
    lv_obj_align(line->label_unit, LV_ALIGN_RIGHT_MID, 0, 0);

    // 데이터 값 라벨을 "uA" 단위 라벨의 왼쪽에 정렬
    lv_obj_align_to(line->label_value, line->label_unit, LV_ALIGN_OUT_LEFT_MID, -5, 0);
}