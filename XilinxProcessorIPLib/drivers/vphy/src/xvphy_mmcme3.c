/*******************************************************************************
* Copyright (C) 2015 - 2020 Xilinx, Inc.  All rights reserved.
* Copyright (C) 2023 Advanced Micro Devices, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
*******************************************************************************/

/******************************************************************************/
/**
 *
 * @file xvphy_mmcme3.c
 *
 * Contains a minimal set of functions for the XVphy driver that allow access
 * to all of the Video PHY core's functionality. See xvphy.h for a detailed
 * description of the driver.
 *
 * @note	None.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -----------------------------------------------
 * 1.7   gm   13/09/17 Initial release.
 * </pre>
 *
*******************************************************************************/

/******************************* Include Files ********************************/

#include "xparameters.h"
#include "xstatus.h"
#include "xvphy.h"
#include "xvphy_i.h"
#if (XPAR_VPHY_0_TRANSCEIVER == XVPHY_GTHE3)

/**************************** Function Prototypes *****************************/
static u32 XVphy_Mmcme3DividerEncoding(XVphy_MmcmDivType DivType, u8 Div);
static u16 XVphy_Mmcme3FilterReg2Encoding(u8 Mult);
static u16 XVphy_Mmcme3LockReg1Encoding(u8 Mult);
static u16 XVphy_Mmcme3LockReg2Encoding(u8 Mult);
static u16 XVphy_Mmcme3LockReg3Encoding(u8 Mult);

/************************** Constant Definitions ******************************/

/*****************************************************************************/
/**
* This function returns the DRP encoding of ClkFbOutMult optimized for:
* Phase = 0; Dutycycle = 0.5; No Fractional division
* The calculations are based on XAPP888
*
* @param	Div is the divider to be encoded
*
* @return
*		- Encoded Value for ClkReg1 [31: 0]
*       - Encoded Value for ClkReg2 [63:32]
*
* @note		None.
*
******************************************************************************/
u32 XVphy_Mmcme3DividerEncoding(XVphy_MmcmDivType DivType, u8 Div)
{
	u32 DrpEnc;
	u32 ClkReg1;
    u32 ClkReg2;
    u8 HiTime, LoTime;

    if (Div == 1) {
        ClkReg1 = 0x00001041;
        ClkReg2 = 0x00C00000;
    }
    else {
        HiTime = Div / 2;
        LoTime = Div - HiTime;

        ClkReg1 = LoTime & 0x3F;
        ClkReg1 |= (HiTime & 0x3F) << 6;
        ClkReg1 |= (DivType == MMCM_DIVCLK_DIVIDE) ? 0x0000 : 0x1000;

        ClkReg2 = (Div % 2) ? 0x00800000 : 0x00000000;
    }

    DrpEnc = ClkReg2 | ClkReg1;

	return DrpEnc;
}

/*****************************************************************************/
/**
* This function returns the DRP encoding of FilterReg1 optimized for:
* Phase = 0; Dutycycle = 0.5; BW = low; No Fractional division
*
* @param	Mult is the divider to be encoded
*
* @return
*		- Encoded Value
*
* @note		None.
*
******************************************************************************/
u16 XVphy_Mmcme3FilterReg2Encoding(u8 Mult)
{
	u16 DrpEnc;

    switch (Mult) {
	case 1: case 2: case 3:
	case 4: case 5: case 6:
		DrpEnc = 0x4990;
		break;
	case 7: case 8: case 9:
    case 13:
		DrpEnc = 0x0990;
		break;
	case 10: case 11: case 12:
	case 17: case 18:
		DrpEnc = 0x4190;
		break;
    case 14: case 15: case 16:
    case 25: case 26: case 27:
    case 28: case 29:
		DrpEnc = 0x0190;
		break;
	case 19: case 20: case 21:
    case 22: case 23: case 24:
	case 40: case 41: case 42:
	case 43: case 44: case 45:
	case 46: case 47:
		DrpEnc = 0x4890;
		break;
    case 30: case 31: case 32:
    case 33: case 34: case 35:
    case 36: case 37: case 38:
    case 39:
		DrpEnc = 0x0890;
		break;
	default:
		DrpEnc = 0x4090;
		break;
	}

	return DrpEnc;
}

/*****************************************************************************/
/**
* This function returns the DRP encoding of LockReg1 optimized for:
* Phase = 0; Dutycycle = 0.5; No Fractional division
*
* @param	Mult is the divider to be encoded
*
* @return
*		- Encoded Value
*
* @note		None.
*
******************************************************************************/
u16 XVphy_Mmcme3LockReg1Encoding(u8 Mult)
{
	u16 DrpEnc;

    switch (Mult) {
	case 1: case 2: case 3:
	case 4: case 5: case 6:
	case 7: case 8: case 9:
	case 10:
		DrpEnc = 0x03e8;
		break;
	case 11:
		DrpEnc = 0x0384;
		break;
	case 12:
		DrpEnc = 0x0339;
		break;
	case 13:
		DrpEnc = 0x02ee;
		break;
	case 14:
		DrpEnc = 0x02bc;
		break;
	case 15:
		DrpEnc = 0x028a;
		break;
	case 16:
		DrpEnc = 0x0271;
		break;
	case 17:
		DrpEnc = 0x023f;
		break;
	case 18:
		DrpEnc = 0x0226;
		break;
	case 19:
		DrpEnc = 0x020d;
		break;
	case 20:
		DrpEnc = 0x01f4;
		break;
	case 21:
		DrpEnc = 0x01db;
		break;
	case 22:
		DrpEnc = 0x01c2;
		break;
	case 23:
		DrpEnc = 0x01a9;
		break;
	case 24:
	case 25:
		DrpEnc = 0x0190;
		break;
	case 26:
		DrpEnc = 0x0177;
		break;
	case 27:
	case 28:
		DrpEnc = 0x015e;
		break;
	case 29:
	case 30:
		DrpEnc = 0x0145;
		break;
	case 31:
	case 32:
	case 33:
		DrpEnc = 0x012c;
		break;
	case 34:
	case 35:
	case 36:
		DrpEnc = 0x0113;
		break;
	default:
		DrpEnc = 0x00fa;
		break;
	}

	return DrpEnc;
}

/*****************************************************************************/
/**
* This function returns the DRP encoding of LockReg2 optimized for:
* Phase = 0; Dutycycle = 0.5; No Fractional division
*
* @param	Mult is the divider to be encoded
*
* @return
*		- Encoded Value
*
* @note		None.
*
******************************************************************************/
u16 XVphy_Mmcme3LockReg2Encoding(u8 Mult)
{
	u16 DrpEnc;

    switch (Mult) {
	case 1:
	case 2:
		DrpEnc = 0x1801;
		break;
	case 3:
		DrpEnc = 0x2001;
		break;
	case 4:
		DrpEnc = 0x2c01;
		break;
	case 5:
		DrpEnc = 0x3801;
		break;
	case 6:
		DrpEnc = 0x4401;
		break;
	case 7:
		DrpEnc = 0x4c01;
		break;
	case 8:
		DrpEnc = 0x5801;
		break;
	case 9:
		DrpEnc = 0x6401;
		break;
	case 10:
		DrpEnc = 0x7001;
		break;
	default:
		DrpEnc = 0x7c01;
		break;
	}

	return DrpEnc;
}

/*****************************************************************************/
/**
* This function returns the DRP encoding of LockReg3 optimized for:
* Phase = 0; Dutycycle = 0.5; No Fractional division
*
* @param	Mult is the divider to be encoded
*
* @return
*		- Encoded Value
*
* @note		None.
*
******************************************************************************/
u16 XVphy_Mmcme3LockReg3Encoding(u8 Mult)
{
	u16 DrpEnc;

    switch (Mult) {
	case 1:
	case 2:
		DrpEnc = 0x1be9;
		break;
	case 3:
		DrpEnc = 0x23e9;
		break;
	case 4:
		DrpEnc = 0x2fe9;
		break;
	case 5:
		DrpEnc = 0x3be9;
		break;
	case 6:
		DrpEnc = 0x47e9;
		break;
	case 7:
		DrpEnc = 0x4fe9;
		break;
	case 8:
		DrpEnc = 0x5be9;
		break;
	case 9:
		DrpEnc = 0x67e9;
		break;
	case 10:
		DrpEnc = 0x73e9;
		break;
	default:
		DrpEnc = 0x7fe9;
		break;
	}

	return DrpEnc;
}

/*****************************************************************************/
/**
* This function will write the mixed-mode clock manager (MMCM) values currently
* stored in the driver's instance structure to hardware .
*
* @param	InstancePtr is a pointer to the XVphy core instance.
* @param	QuadId is the GT quad ID to operate on.
* @param	Dir is an indicator for TX or RX.
*
* @return
*		- XST_SUCCESS if the MMCM write was successful.
*		- XST_FAILURE otherwise, if the configuration success bit did
*		  not go low.
*
* @note		None.
*
******************************************************************************/
u32 XVphy_MmcmWriteParameters(XVphy *InstancePtr, u8 QuadId,
							XVphy_DirectionType Dir)
{
	u8 ChId;
	u16 DrpVal;
	u32 DrpVal32;
	XVphy_Mmcm *MmcmParams;

    ChId = (Dir == XVPHY_DIR_TX) ?
						XVPHY_CHANNEL_ID_TXMMCM :
						XVPHY_CHANNEL_ID_RXMMCM;

	MmcmParams = &InstancePtr->Quads[QuadId].Mmcm[Dir];

	/* Check Parameters if has been Initialized */
	if (!MmcmParams->DivClkDivide && !MmcmParams->ClkFbOutMult &&
			!MmcmParams->ClkOut0Div && !MmcmParams->ClkOut1Div &&
			!MmcmParams->ClkOut2Div) {
		return XST_FAILURE;
	}


	/* Write Power Register Value */
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x27, 0xFFFF);

	/* Write CLKFBOUT Reg1 & Reg2 Values */
	DrpVal32 = XVphy_Mmcme3DividerEncoding(MMCM_CLKFBOUT_MULT_F,
						MmcmParams->ClkFbOutMult);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x14,
						(u16)(DrpVal32 & 0xFFFF));
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x15,
						(u16)((DrpVal32 >> 16) & 0xFFFF));

	/* Write DIVCLK_DIVIDE Value */
	DrpVal32 = XVphy_Mmcme3DividerEncoding(MMCM_DIVCLK_DIVIDE,
						MmcmParams->DivClkDivide) ;
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x16,
						(u16)(DrpVal32 & 0xFFFF));

	/* Write CLKOUT0 Reg1 & Reg2 Values */
	DrpVal32 = XVphy_Mmcme3DividerEncoding(MMCM_CLKOUT_DIVIDE,
						MmcmParams->ClkOut0Div);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x08,
						(u16)(DrpVal32 & 0xFFFF));
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x09,
						(u16)((DrpVal32 >> 16) & 0xFFFF));

	/* Write CLKOUT1 Reg1 & Reg2 Values */
	DrpVal32 = XVphy_Mmcme3DividerEncoding(MMCM_CLKOUT_DIVIDE,
						MmcmParams->ClkOut1Div);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x0A,
						(u16)(DrpVal32 & 0xFFFF));
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x0B,
						(u16)((DrpVal32 >> 16) & 0xFFFF));

	/* Write CLKOUT2 Reg1 & Reg2 Values */
	DrpVal32 = XVphy_Mmcme3DividerEncoding(MMCM_CLKOUT_DIVIDE,
						MmcmParams->ClkOut2Div);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x0C,
						(u16)(DrpVal32 & 0xFFFF));
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x0D,
						(u16)((DrpVal32 >> 16) & 0xFFFF));

	/* Write Lock Reg1 Value */
	DrpVal = XVphy_Mmcme3LockReg1Encoding(MmcmParams->ClkFbOutMult);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x18, DrpVal);

	/* Write Lock Reg2 Value */
	DrpVal = XVphy_Mmcme3LockReg2Encoding(MmcmParams->ClkFbOutMult);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x19, DrpVal);

	/* Write Lock Reg3 Value */
	DrpVal = XVphy_Mmcme3LockReg3Encoding(MmcmParams->ClkFbOutMult);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x1A, DrpVal);

	/* Write Filter Reg1 Value */
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x4E, 0x0800);

	/* Write Filter Reg2 Value */
	DrpVal = XVphy_Mmcme3FilterReg2Encoding(MmcmParams->ClkFbOutMult);
	XVphy_DrpWr(InstancePtr, QuadId, ChId, 0x4F, DrpVal);

	return XST_SUCCESS;
}

#endif
