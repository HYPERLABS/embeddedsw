# Copyright 2024 Advanced Micro Devices, Inc. All Rights Reserved.
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.14.7)
project(dsirxss)

find_package(common)
collector_create (PROJECT_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")
collector_create (PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${CMAKE_BINARY_DIR}/include)
collect (PROJECT_LIB_SOURCES xdsi2rxss.c)
collect (PROJECT_LIB_HEADERS xdsi2rxss.h)
collect (PROJECT_LIB_SOURCES xdsi2rxss_g.c)
collect (PROJECT_LIB_HEADERS xdsi2rxss_hw.h)
collect (PROJECT_LIB_SOURCES xdsi2rxss_selftest.c)
collect (PROJECT_LIB_SOURCES xdsi2rxss_intr.c)
collect (PROJECT_LIB_SOURCES xdsi2rxss_sinit.c)
collector_list (_sources PROJECT_LIB_SOURCES)
collector_list (_headers PROJECT_LIB_HEADERS)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
add_library(dsirxss STATIC ${_sources})
set_target_properties(dsirxss PROPERTIES LINKER_LANGUAGE C)
