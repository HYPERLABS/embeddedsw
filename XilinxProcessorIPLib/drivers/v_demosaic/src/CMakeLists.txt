# Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.14.7)
project(v_demosaic)

find_package(common)
collector_create (PROJECT_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")
collector_create (PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${CMAKE_BINARY_DIR}/include)
collect (PROJECT_LIB_SOURCES xv_demosaic.c)
collect (PROJECT_LIB_HEADERS xv_demosaic.h)
collect (PROJECT_LIB_SOURCES xv_demosaic_g.c)
collect (PROJECT_LIB_HEADERS xv_demosaic_hw.h)
collect (PROJECT_LIB_SOURCES xv_demosaic_linux.c)
collect (PROJECT_LIB_SOURCES xv_demosaic_sinit.c)
collector_list (_sources PROJECT_LIB_SOURCES)
collector_list (_headers PROJECT_LIB_HEADERS)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
add_library(v_demosaic STATIC ${_sources})
set_target_properties(v_demosaic PROPERTIES LINKER_LANGUAGE C)
