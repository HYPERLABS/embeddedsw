# Copyright (C) 2024 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT

if("${XILPLMI_mode}" STREQUAL "server")
  add_subdirectory(server)
endif()