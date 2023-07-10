/******************************************************************************
* Copyright (c) 2022 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xsecure_plat_kat_ipihandler.c
* @addtogroup xsecure_apis XilSecure versal net KAT handler APIs
* @{
* @cond xsecure_internal
* This file contains the xilsecure KAT IPI handlers implementation.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date       Changes
* ----- ---- ---------- -------------------------------------------------------
* 1.00  kpt  07/15/2022 Initial release
* 1.01  kpt  01/04/2023 Added API to set or clear KAT status for external modules
* 1.02  ng   05/10/2023 Removed XSecure_PerformKatOperation and implemented
*                       redundant call for XPlmi_ClearKatMask
* 5.2   vns  07/06/2023 Separated the IPI commands of Update Kat Status
* </pre>
*
* @note
* @endcond
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xplmi_dma.h"
#include "xsecure_plat_kat.h"
#include "xsecure_kat_ipihandler.h"
#include "xsecure_plat_kat_ipihandler.h"
#include "xsecure_defs.h"
#include "xsecure_error.h"
#include "xil_util.h"
#include "xsecure_init.h"
#include "xplmi.h"

/************************** Constant Definitions *****************************/

#define XSECURE_KAT_MAX_CMD_LEN		(5U)	/**< Maximum command length*/
#define XSECURE_DDR_KAT_MASK_MUL	(4U)	/**< Multiplier to get DDR KAT mask */
#define XSECURE_DDR_MAX_SUPPORT		(7U)	/**< Maximum supported DDR */

/************************** Function Prototypes *****************************/
#ifndef PLM_SECURE_EXCLUDE
static int XSecure_TrngKat(void);
#endif
/*****************************************************************************/
/**
 * @brief       This function calls respective IPI handler based on the API_ID
 *
 * @param 	Cmd is pointer to the command structure
 *
 * @return
 *	-	XST_SUCCESS - If the handler execution is successful
 *	-	ErrorCode - If there is a failure
 *
 ******************************************************************************/
int XSecure_KatPlatIpiHandler(XPlmi_Cmd *Cmd)
{
	volatile int Status = XST_FAILURE;
	u32 *Pload = Cmd->Payload;

	switch (Pload[0U] & XSECURE_API_ID_MASK) {
#ifndef PLM_SECURE_EXCLUDE
	case XSECURE_API(XSECURE_API_TRNG_KAT):
		Status = XSecure_TrngKat();
		break;
#endif
	default:
		/* Common IPI handler for versal devices */
		Status = XSecure_KatIpiHandler(Cmd);
		break;
	}

	return Status;
}

#ifndef PLM_SECURE_EXCLUDE
/*****************************************************************************/
/**
 * @brief       This function handler calls XSecure_TrngPreOperationalSelfTests Server API
 *
 * @return	- XST_SUCCESS - If the KAT is successful
 * 		- ErrorCode - If there is a failure
 *
 ******************************************************************************/
static int XSecure_TrngKat(void)
{
	volatile int Status = XST_FAILURE;
	XSecure_TrngInstance *TrngInstance = XSecure_GetTrngInstance();

	Status = XSecure_TrngPreOperationalSelfTests(TrngInstance);
	/* Update KAT status in to RTC area */
	if (Status != XST_SUCCESS) {
		XSECURE_REDUNDANT_IMPL(XPlmi_ClearKatMask, XPLMI_SECURE_TRNG_KAT_MASK);
	}
	else {
		XPlmi_SetKatMask(XPLMI_SECURE_TRNG_KAT_MASK);
	}

	return Status;
}
#endif

/*****************************************************************************/
/**
 * @brief	This function calls respective IPI handler based on the API_ID
 *
 * @param	Cmd is pointer to the command structure
 *
 * @return
 *		 - XST_SUCCESS  On Success
 *		 - Error Code  On failure
 *
 ******************************************************************************/
int XSecure_UpdateKatStatusIpiHandler(XPlmi_Cmd *Cmd)
{
	int Status = XST_FAILURE;
	u32 KatAddr = 0U;
	u32 KatVal[XSECURE_MAX_KAT_MASK_LEN] = {0U};
	u32 KatMaskLen = 0U;
	u32 *Pload = Cmd->Payload;
	u32 KatOp = Pload[0U];
	u32 *UserKatMask = &Pload[1U];
	u32 KatMask;

	if ((KatOp != (u32)XSECURE_API_KAT_CLEAR) && (KatOp != (u32)XSECURE_API_KAT_SET)) {
			goto END;
	}

	if ((Cmd->Len <= XSECURE_KAT_HDR_LEN) || (Cmd->Len > XSECURE_KAT_MAX_CMD_LEN)) {
		goto END;
	}

	KatMaskLen = Cmd->Len - XSECURE_KAT_HDR_LEN;
	if (KatMaskLen == 0U) {
		goto END;
	}
	switch (Cmd->CmdId & XSECURE_API_ID_MASK) {
	case XSECURE_API(XSECURE_API_UPDATE_DDR_KAT_STATUS):
		if (Pload[2] > XSECURE_DDR_MAX_SUPPORT) {
			Status = XST_INVALID_PARAM;
			goto END;
		}
		KatAddr = XPLMI_RTCFG_SECURE_DDR_KAT_ADDR;
		KatMask = XPLMI_DDR_0_KAT_MASK << (XSECURE_DDR_KAT_MASK_MUL * Pload[2]);
		break;
	case XSECURE_API(XSECURE_API_UPDATE_HNIC_KAT_STATUS):
		KatAddr = XPLMI_RTCFG_SECURE_HNIC_CPM5N_PCIDE_KAT_ADDR;
		KatMask =  XPLMI_HNIC_KAT_MASK;
		break;
	case XSECURE_API(XSECURE_API_UPDATE_CPM5N_KAT_STATUS):
		KatAddr = XPLMI_RTCFG_SECURE_HNIC_CPM5N_PCIDE_KAT_ADDR;
		KatMask = XPLMI_CPM5N_KAT_MASK;
		break;
	case XSECURE_API(XSECURE_API_UPDATE_PCIDE_KAT_STATUS):
		KatAddr = XPLMI_RTCFG_SECURE_HNIC_CPM5N_PCIDE_KAT_ADDR;
		KatMask = XPLMI_PCIDE_KAT_MASK;
		break;
	case XSECURE_API(XSECURE_API_UPDATE_PKI_KAT_STATUS):
		if (KatMaskLen == XSECURE_MAX_KAT_MASK_LEN) {
			KatAddr = XPLMI_RTCFG_SECURE_PKI_KAT_ADDR_0;
			KatMask = XPLMI_PKI_KAT_MASK;
		}
		else {
			Status = XST_FAILURE;
			goto END;
		}
		break;

	default:
		KatAddr = 0U;
		XSecure_Printf(XSECURE_DEBUG_GENERAL,"Invalid request for KAT updation \n\r");
		break;
	}

	if (KatAddr != 0U) {
		if (KatOp != (u32)XSECURE_API_KAT_CLEAR) {
			KatVal[0U] = UserKatMask[0U] & KatMask;
			if (KatAddr == XPLMI_RTCFG_SECURE_PKI_KAT_ADDR_0) {
				KatVal[1U] = UserKatMask[1U] & KatMask;
				KatVal[2U] = UserKatMask[2U] & KatMask;
			}
		}
		else {
			KatVal[0U] = ~(UserKatMask[0U] & KatMask);
			if (KatAddr == XPLMI_RTCFG_SECURE_PKI_KAT_ADDR_0) {
				KatVal[1U] = ~(UserKatMask[1U] & KatMask);
				KatVal[2U] = ~(UserKatMask[2U] & KatMask);
			}
		}

		Status = Xil_SecureRMW32(KatAddr, KatMask, KatVal[0U]);
		if (Status != XST_SUCCESS) {
			goto END;
		}
		if (KatAddr == XPLMI_RTCFG_SECURE_PKI_KAT_ADDR_0) {
			Status = Xil_SecureRMW32(XPLMI_RTCFG_SECURE_PKI_KAT_ADDR_1, KatMask, KatVal[1U]);
			if (Status != XST_SUCCESS) {
				goto END;
			}
			Status = Xil_SecureRMW32(XPLMI_RTCFG_SECURE_PKI_KAT_ADDR_2, KatMask, KatVal[2U]);
			if (Status != XST_SUCCESS) {
				goto END;
			}

		}
		Status = XPlmi_CheckAndUpdateFipsState();
	}

END:
	return Status;
}
