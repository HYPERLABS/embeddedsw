/******************************************************************************
* Copyright (C) 2018 – 2020 Xilinx, Inc.  All rights reserved.
* Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/


#ifndef SDT
#include "xparameters.h"
#endif
#include "xv_hdmitx1.h"

/*
* The configuration table for devices
*/

XV_HdmiTx1_Config XV_HdmiTx1_ConfigTable[XPAR_XV_HDMITX1_NUM_INSTANCES] =
{
	{
		XPAR_V_HDMI_TXSS1_V_HDMI_TX_DEVICE_ID,
		XPAR_V_HDMI_TXSS1_V_HDMI_TX_BASEADDR,
		XPAR_V_HDMI_TXSS1_V_HDMI_TX_MAX_FRL_RATE,
		XPAR_V_HDMI_TXSS1_V_HDMI_TX_DYNAMIC_HDR,
		XPAR_V_HDMI_TXSS1_V_HDMI_TX_AXI_LITE_FREQ_HZ
	}
};


