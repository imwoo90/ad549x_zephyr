target_sources(app PRIVATE ./ad5940lib/ad5940.c)

target_include_directories(app PRIVATE
    ad5940lib
)