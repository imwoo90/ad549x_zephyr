target_sources(app PRIVATE
    battery_status_ui.c
    mesurement_ui.c
    ui_main.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)