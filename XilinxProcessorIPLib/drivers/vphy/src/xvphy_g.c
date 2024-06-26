/*******************************************************************
* Copyright (C) 2010-2020 Xilinx, Inc. All rights reserved.
* Copyright (C) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
*******************************************************************/

/******************************************************************/

#include "xparameters.h"
#include "xvphy.h"

/*
* The configuration table for devices
*/

XVphy_Config XVphy_ConfigTable[XPAR_XVPHY_NUM_INSTANCES] =
{
	{
		XPAR_VID_PHY_CONTROLLER_DEVICE_ID,
		XPAR_VID_PHY_CONTROLLER_BASEADDR,
		(XVphy_GtType)XPAR_VID_PHY_CONTROLLER_TRANSCEIVER,
		XPAR_VID_PHY_CONTROLLER_TX_NO_OF_CHANNELS,
		XPAR_VID_PHY_CONTROLLER_RX_NO_OF_CHANNELS,
		(XVphy_ProtocolType)XPAR_VID_PHY_CONTROLLER_TX_PROTOCOL,
		(XVphy_ProtocolType)XPAR_VID_PHY_CONTROLLER_RX_PROTOCOL,
		(XVphy_PllRefClkSelType)XPAR_VID_PHY_CONTROLLER_TX_REFCLK_SEL,
		(XVphy_PllRefClkSelType)XPAR_VID_PHY_CONTROLLER_RX_REFCLK_SEL,
		(XVphy_SysClkDataSelType)XPAR_VID_PHY_CONTROLLER_TX_PLL_SELECTION,
		(XVphy_SysClkDataSelType)XPAR_VID_PHY_CONTROLLER_RX_PLL_SELECTION,
		XPAR_VID_PHY_CONTROLLER_NIDRU,
		(XVphy_PllRefClkSelType)XPAR_VID_PHY_CONTROLLER_NIDRU_REFCLK_SEL,
		(XVidC_PixelsPerClock)XPAR_VID_PHY_CONTROLLER_INPUT_PIXELS_PER_CLOCK,
		XPAR_VID_PHY_CONTROLLER_TX_BUFFER_BYPASS,
		XPAR_VID_PHY_CONTROLLER_HDMI_FAST_SWITCH,
		XPAR_VID_PHY_CONTROLLER_TRANSCEIVER_WIDTH,
		XPAR_VID_PHY_CONTROLLER_ERR_IRQ_EN,
		XPAR_VID_PHY_CONTROLLER_AXI_LITE_FREQ_HZ,
		XPAR_VID_PHY_CONTROLLER_DRPCLK_FREQ,
		XPAR_VID_PHY_CONTROLLER_USE_GT_CH4_HDMI,
                XPAR_VID_PHY_CONTROLLER_TX_DP_PROTOCOL,
                XPAR_VID_PHY_CONTROLLER_RX_DP_PROTOCOL,
	}
};
