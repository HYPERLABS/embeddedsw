# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
set( CMAKE_EXPORT_COMPILE_COMMANDS ON)
set( CMAKE_TRY_COMPILE_PLATFORM_VARIABLES CMAKE_LIBRARY_PATH)
set( CMAKE_INSTALL_MESSAGE LAZY)
set( CMAKE_C_COMPILER mb-gcc )
set( CMAKE_CXX_COMPILER mb-g++ )
set( CMAKE_C_COMPILER_LAUNCHER  )
set( CMAKE_CXX_COMPILER_LAUNCHER  )
set( CMAKE_ASM_COMPILER mb-gcc )
set( CMAKE_AR mb-ar CACHE FILEPATH "Archiver" )
set( CMAKE_SIZE mb-size CACHE FILEPATH "Size" )
set( CMAKE_SYSTEM_PROCESSOR "pmu_microblaze" )
set( CMAKE_MACHINE "ZynqMP" )
set( CMAKE_SYSTEM_NAME "Generic" )
set( CMAKE_SPECS_FILE "$ENV{ESW_REPO}/scripts/specs/microblaze/Xilinx.spec" CACHE STRING "Specs file path for using CMAKE toolchain files" )
# FIXME: Use generic microblaze toolchain file for pmu microblaze processor as well.
set( TOOLCHAIN_PMU_C_FLAGS " -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mxl-reorder -mxl-soft-mul -mxl-soft-div -O2 -pipe -g -feliminate-unused-debug-types -Os -flto -ffat-lto-objects -DPSU_PMU=1U -mcpu=v9.2")
set( TOOLCHAIN_C_FLAGS " -O2 ${TOOLCHAIN_PMU_C_FLAGS} -DSDT" CACHE STRING "CFLAGS" )
set( TOOLCHAIN_CXX_FLAGS " -O2 ${TOOLCHAIN_PMU_C_FLAGS} -DSDT" CACHE STRING "CXXFLAGS" )
set( TOOLCHAIN_ASM_FLAGS " -O2 ${TOOLCHAIN_PMU_C_FLAGS} -DSDT" CACHE STRING "ASM FLAGS" )
set( TOOLCHAIN_DEP_FLAGS " -MMD -MP" CACHE STRING "Flags used by compiler to generate dependency files")
set( TOOLCHAIN_EXTRA_C_FLAGS " -g -ffunction-sections -fdata-sections -Wall -Wextra -fno-tree-loop-distribute-patterns" CACHE STRING "Extra CFLAGS")
set( CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CFLAGS for release" )
set( CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CFLAGS for release" )
set( CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional CXXFLAGS for release" )
set( CMAKE_ASM_FLAGS_RELEASE "-DNDEBUG" CACHE STRING "Additional ASM FLAGS for release" )
