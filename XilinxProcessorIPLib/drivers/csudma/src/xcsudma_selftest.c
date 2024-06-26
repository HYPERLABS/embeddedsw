/******************************************************************************
* Copyright (C) 2014 - 2022 Xilinx, Inc.  All rights reserved.
* Copyright (C) 2022 - 2023 Advanced Micro Devices, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/


/*****************************************************************************/
/**
*
* @file xcsudma_selftest.c
* @addtogroup csuma_api CSUDMA APIs
* @{
*
* The xcsudma_selftest.c file contains a diagnostic self-test function for the CSU_DMA driver.
* Refer to the header file xcsudma.h for detailed information.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who     Date     Changes
* ----- ------  -------- ---------------------------------------------------
* 1.0   vnsld   22/10/14 First release
* 1.11	sk	03/03/22 Replace driver version in addtogroup with Overview.
* 1.11	sk	03/03/22 Update Overview section based on review comments.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xcsudma.h"

/************************** Constant Definitions ****************************/


/**************************** Type Definitions ******************************/


/***************** Macros (Inline Functions) Definitions ********************/


/************************** Variable Definitions ****************************/


/************************** Function Prototypes *****************************/


/************************** Function Definitions *****************************/


/*****************************************************************************/
/**
*
* This function runs a self-test on the driver and hardware device. Performs
* reset of both source and destination channels and checks if reset is working
* properly or not.
*
* @param	InstancePtr Pointer to the XCsuDma instance.
*
* @return
*		- XST_SUCCESS if the self-test passed.
* 		- XST_FAILURE otherwise.
*
* @note		None.
*
******************************************************************************/
s32 XCsuDma_SelfTest(XCsuDma *InstancePtr)
{
	u32 Data;
	s32 Status;

	/* Verify arguments. */
	Xil_AssertNonvoid(InstancePtr != NULL);

	Data = XCsuDma_ReadReg(InstancePtr->Config.BaseAddress,
					(u32)(XCSUDMA_CTRL_OFFSET));

	/* Changing Endianess of Source channel */

	XCsuDma_WriteReg(InstancePtr->Config.BaseAddress,
			(u32)(XCSUDMA_CTRL_OFFSET),
			((Data) | (u32)(XCSUDMA_CTRL_ENDIAN_MASK)));

	if ((XCsuDma_ReadReg(InstancePtr->Config.BaseAddress,
		(u32)(XCSUDMA_CTRL_OFFSET)) &
			(u32)(XCSUDMA_CTRL_ENDIAN_MASK)) ==
				(XCSUDMA_CTRL_ENDIAN_MASK)) {
		Status = (s32)(XST_SUCCESS);
	}
	else {
		Status = (s32)(XST_FAILURE);
	}

	/* Changes made are being reverted back */
	XCsuDma_WriteReg(InstancePtr->Config.BaseAddress,
				(u32)(XCSUDMA_CTRL_OFFSET), Data);

	return Status;

}
/** @} */
