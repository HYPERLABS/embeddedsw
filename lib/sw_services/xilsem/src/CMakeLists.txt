# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.14.7)
project(xilsem)

find_package(common)
include(${CMAKE_CURRENT_SOURCE_DIR}/xilsem.cmake NO_POLICY_SCOPE)
collector_create (PROJECT_LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}")
collector_create (PROJECT_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${CMAKE_BINARY_DIR}/include)

if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "plm_microblaze")
add_subdirectory(server)
collector_list (_headers PROJECT_LIB_HEADERS)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
else()
message(STATUS "XILSEM client")
add_subdirectory(client)
collector_list (_headers PROJECT_LIB_HEADERS)
collector_list (_sources PROJECT_LIB_SOURCES)
file(COPY ${_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
if (NOT ${YOCTO})
file(COPY ${CMAKE_BINARY_DIR}/include/xilsem_config.h DESTINATION ${CMAKE_INCLUDE_PATH}/)
endif()
add_library(xilsem STATIC ${_sources})
get_headers(${_headers})
set_target_properties(xilsem PROPERTIES LINKER_LANGUAGE C)
set_target_properties(xilsem PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_LIBRARY_PATH}/libxilsem.a;${CMAKE_INCLUDE_PATH}/xilsem_config.h;${clean_headers}")
install(TARGETS xilsem LIBRARY DESTINATION ${CMAKE_LIBRARY_PATH} ARCHIVE DESTINATION ${CMAKE_LIBRARY_PATH})
install(DIRECTORY ${CMAKE_BINARY_DIR}/include DESTINATION ${CMAKE_INCLUDE_PATH}/..)
endif()
