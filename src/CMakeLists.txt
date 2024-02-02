target_sources(app PRIVATE
    main.c
    ad5940ZephyrPort.c
)

add_subdirectory_ifdef(CONFIG_SQR_WAVE_VOLTAMMETRY SqrWaveVoltammetry)
add_subdirectory_ifdef(CONFIG_CHRONO_AMPEROMETRIC ChronoAmperometric)