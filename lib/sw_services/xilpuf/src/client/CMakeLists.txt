# Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT

if (NOT ("xilmailbox" IN_LIST BSP_LIBSRC_SUBDIRS))
  message(WARNING "XilPuf library requires XilMailbox library in client mode.")
endif()

collect (PROJECT_LIB_HEADERS xpuf_client.h)
collect (PROJECT_LIB_SOURCES xpuf_client.c)

collect (PROJECT_LIB_HEADERS xpuf_mailbox.h)
collect (PROJECT_LIB_SOURCES xpuf_mailbox.c)
