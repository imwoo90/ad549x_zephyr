target_sources(app PRIVATE
    queue.c
    battery.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)