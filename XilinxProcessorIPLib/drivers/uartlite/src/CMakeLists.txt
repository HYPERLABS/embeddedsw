# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# Copyright (c) 2021 Xilinx, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.15)
project(uartlite)

find_package(common)
# If the PG flag is present, remove any references to it as BSP doesn't support the mcount API.
remove_pg()

collector_create (PROJECT_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")
collector_create (PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${CMAKE_BINARY_DIR}/include)
collect (PROJECT_LIB_SOURCES xuartlite.c)
collect (PROJECT_LIB_HEADERS xuartlite.h)
collect (PROJECT_LIB_SOURCES xuartlite_g.c)
collect (PROJECT_LIB_HEADERS xuartlite_i.h)
collect (PROJECT_LIB_SOURCES xuartlite_intr.c)
collect (PROJECT_LIB_SOURCES xuartlite_l.c)
collect (PROJECT_LIB_HEADERS xuartlite_l.h)
collect (PROJECT_LIB_SOURCES xuartlite_selftest.c)
collect (PROJECT_LIB_SOURCES xuartlite_sinit.c)
collect (PROJECT_LIB_SOURCES xuartlite_stats.c)
collector_list (_sources PROJECT_LIB_SOURCES)
collector_list (_headers PROJECT_LIB_HEADERS)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
add_library(uartlite STATIC ${_sources})
set_target_properties(uartlite PROPERTIES LINKER_LANGUAGE C)
