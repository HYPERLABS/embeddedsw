/******************************************************************************
* Copyright (C) 2015 - 2020 Xilinx, Inc. All rights reserved.
* Copyright (c) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xdptxss_hdcp1x.h
*
* This is the header file for Xilinx DisplayPort Transmitter Subsystem
* sub-core, is High-Bandwidth Content Protection (HDCP).
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver  Who Date     Changes
* ---- --- -------- -----------------------------------------------------
* 2.00 sha 09/28/15 Initial release.
* </pre>
*
******************************************************************************/
#ifndef XDPTXSS_HDCP1X_H_
#define XDPTXSS_HDCP1X_H_	/**< Prevent circular inclusions
				  *  by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xparameters.h"
#ifdef SDT
#define XPAR_XHDCP_NUM_INSTANCES XPAR_XHDCP1X_NUM_INSTANCES
#endif
#if (XPAR_XHDCP_NUM_INSTANCES > 0)
#include "xhdcp1x.h"
#include "xhdcp1x_port.h"
#endif
#include "xtmrctr.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Declarations ****************************/


#ifdef __cplusplus
}
#endif

#endif /* End of protection macro */
