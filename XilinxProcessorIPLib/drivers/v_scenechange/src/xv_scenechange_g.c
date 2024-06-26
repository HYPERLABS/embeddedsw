
/*******************************************************************
* Copyright (c) 2010-2022 Xilinx, Inc. All Rights Reserved.
* Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
*******************************************************************/

#ifndef SDT
#include "xparameters.h"
#endif
#include "xv_scenechange.h"

/*
* The configuration table for devices
*/

XV_scenechange_Config XV_scenechange_ConfigTable[] =
{
	{
		XPAR_V_SCENECHANGE_0_DEVICE_ID,
		XPAR_V_SCENECHANGE_0_S_AXI_CTRL_BASEADDR,
		XPAR_XV_SCENECHANGE_0_MEMORY_BASED,
		XPAR_XV_SCENECHANGE_0_MAX_NR_STREAMS,
		XPAR_XV_SCENECHANGE_0_HISTOGRAM_BITS,
		XPAR_XV_SCENECHANGE_0_HAS_Y10,
		XPAR_XV_SCENECHANGE_0_HAS_Y8,
		XPAR_XV_SCENECHANGE_0_MAX_ROWS,
		XPAR_XV_SCENECHANGE_0_MAX_COLS
	}
};
