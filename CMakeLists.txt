#
# Copyright (c) 2019 TDK Invensense
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(icm42605)

target_compile_definitions(app PRIVATE
    CHIPSEL_594X
)

add_subdirectory(lib)
add_subdirectory(src)

