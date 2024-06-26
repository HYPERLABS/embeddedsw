# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.14.7)

project(dp14)

find_package(common)
collector_create (PROJECT_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")
collector_create (PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${CMAKE_BINARY_DIR}/include)
collect (PROJECT_LIB_SOURCES xdp_edid.c)
collect (PROJECT_LIB_SOURCES xdp_g.c)
collect (PROJECT_LIB_HEADERS xdp_hw.h)
collect (PROJECT_LIB_SOURCES xdp_intr.c)
collect (PROJECT_LIB_SOURCES xdp_mst.c)
collect (PROJECT_LIB_SOURCES xdp_selftest.c)
collect (PROJECT_LIB_SOURCES xdp_sinit.c)
collect (PROJECT_LIB_SOURCES xdp_spm.c)
collect (PROJECT_LIB_SOURCES xdp.c)
collect (PROJECT_LIB_HEADERS xdp.h)
collector_list (_sources PROJECT_LIB_SOURCES)
collector_list (_headers PROJECT_LIB_HEADERS)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
add_library(dp14 STATIC ${_sources})
set_target_properties(dp14 PROPERTIES LINKER_LANGUAGE C)
