target_sources(app PRIVATE
    main.c
    gpio_interrupt_handler.cpp
)

add_subdirectory(ad5940)
add_subdirectory_ifdef(CONFIG_ADC battery)
add_subdirectory_ifdef(CONFIG_DISPLAY UI)