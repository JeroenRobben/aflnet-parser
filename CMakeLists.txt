cmake_minimum_required(VERSION 3.15)
project(aflnet_parser C)

set(CMAKE_C_STANDARD 11)

add_compile_definitions(_GNU_SOURCE)
add_compile_options(-g)
add_link_options(-g)

SET(SRC_FILES
        main.c
        lib/aflnet/aflnet.c
)

set_source_files_properties(
        lib/aflnet/aflnet.c
        PROPERTIES
        COMPILE_FLAGS "-Wno-pointer-sign -Wno-pointer-to-int-cast"
)

add_executable(aflnet-parser ${SRC_FILES})