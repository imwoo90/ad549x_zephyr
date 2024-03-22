target_sources(app PRIVATE
    ad5940_main.c
    ad5940_state.c
    ad5940ZephyrPort.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory_ifdef(CONFIG_SQR_WAVE_VOLTAMMETRY SqrWaveVoltammetry)
add_subdirectory_ifdef(CONFIG_CHRONO_AMPEROMETRIC ChronoAmperometric)