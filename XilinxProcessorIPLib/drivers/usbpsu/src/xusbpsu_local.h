/******************************************************************************
* Copyright (C) 2020 - 2022 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2023 - 2024 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/****************************************************************************/
/**
*
* @file xusbpsu_local.h
* @addtogroup usbpsu_api USBPSU APIs
* @{
* @details
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- -----  -------- -----------------------------------------------------
* 1.0   pm    03/23/20 First release
* 1.9   pm    03/15/21 Fixed doxygen warnings
*
* </pre>
*
*****************************************************************************/
/** @cond INTERNAL */
#ifndef XUSBPSU_LOCAL_H  /* Prevent circular inclusions */
#define XUSBPSU_LOCAL_H  /**< by using protection macros  */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "xusbpsu.h"

/************************** Constant Definitions *****************************/
#ifdef XUSBPSU_HIBERNATION_ENABLE  /**< HIBERNATION Flag ENABLE */

#define XUSBPSU_NON_STICKY_SAVE_RETRIES		500U
#define XUSBPSU_PWR_STATE_RETRIES		1500U
#define XUSBPSU_CTRL_RDY_RETRIES		5000U
#define XUSBPSU_TIMEOUT				1000U

#endif
/************************** Function Prototypes ******************************/

/*
 * Functions in xusbpsu.c
 */
u8 XUsbPsu_GetLinkState(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_SetLinkState(struct XUsbPsu *InstancePtr,
			 XusbPsuLinkStateChange State);

/*
 * Functions in xusbpsu_device.c
 */
s32 XUsbPsu_WaitClearTimeout(struct XUsbPsu *InstancePtr, u32 Offset,
			     u32 BitMask, u32 Timeout);
s32 XUsbPsu_WaitSetTimeout(struct XUsbPsu *InstancePtr, u32 Offset,
			   u32 BitMask, u32 Timeout);
u32 XUsbPsu_ReadHwParams(struct XUsbPsu *InstancePtr, u8 RegIndex);
s32 XUsbPsu_CoreInit(struct XUsbPsu *InstancePtr);
void XUsbPsu_EventBuffersSetup(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_SetTestMode(struct XUsbPsu *InstancePtr, u32 Mode);

/*
 * Functions in xusbpsu_command.c
 */
struct XUsbPsu_EpParams *XUsbPsu_GetEpParams(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_SendEpCmd(struct XUsbPsu *InstancePtr, u8 UsbEpNum, u8 Dir,
		      u32 Cmd, struct XUsbPsu_EpParams *Params);

/*
 * Functions in xusbpsu_ephandler.c
 */
void XUsbPsu_EpTransferDeactive(struct XUsbPsu *InstancePtr, u8 UsbEpNum,
				u8 Dir);
void XUsbPsu_SaveEndpointState(struct XUsbPsu *InstancePtr,
			       struct XUsbPsu_Ep *Ept);
void XUsbPsu_EpXferComplete(struct XUsbPsu *InstancePtr,
			    const struct XUsbPsu_Event_Epevt *Event);
void XUsbPsu_EpXferNotReady(struct XUsbPsu *InstancePtr,
			    const struct XUsbPsu_Event_Epevt *Event);

/*
 * Functions in xusbpsu_endpoint.c
 */
u32 XUsbPsu_EpGetTransferIndex(struct XUsbPsu *InstancePtr, u8 UsbEpNum,
			       u8 Dir);
s32 XUsbPsu_StartEpConfig(struct XUsbPsu *InstancePtr, u32 UsbEpNum,
			  u8 Dir);
s32 XUsbPsu_SetEpConfig(struct XUsbPsu *InstancePtr, u8 UsbEpNum, u8 Dir,
			u16 Size, u8 Type, u8 Restore);
s32 XUsbPsu_SetXferResource(struct XUsbPsu *InstancePtr, u8 UsbEpNum, u8 Dir);
void XUsbPsu_ClearStallAllEp(struct XUsbPsu *InstancePtr);
void XUsbPsu_StopActiveTransfers(struct XUsbPsu *InstancePtr);

/*
 * Functions in xusbpsu_controltransfer.c
 */
void XUsbPsu_Ep0DataDone(struct XUsbPsu *InstancePtr,
			 const struct XUsbPsu_Event_Epevt *Event);
void XUsbPsu_Ep0StatusDone(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_EnableControlEp(struct XUsbPsu *InstancePtr, u16 Size);
s32 XUsbPsu_Ep0StartStatus(struct XUsbPsu *InstancePtr,
			   const struct XUsbPsu_Event_Epevt *Event);
void XUsbPsu_Ep0_EndControlData(struct XUsbPsu *InstancePtr,
				struct XUsbPsu_Ep *Ept);

/*
 * Functions in xusbpsu_ep0handler.c
 */
s32 XUsbPsu_RecvSetup(struct XUsbPsu *InstancePtr);
void XUsbPsu_Ep0XferComplete(struct XUsbPsu *InstancePtr,
			     const struct XUsbPsu_Event_Epevt *Event);
void XUsbPsu_Ep0XferNotReady(struct XUsbPsu *InstancePtr,
			     const struct XUsbPsu_Event_Epevt *Event);
s32 XUsbPsu_Ep0Send(struct XUsbPsu *InstancePtr, u8 *BufferPtr,
		    u32 BufferLen);
s32 XUsbPsu_Ep0Recv(struct XUsbPsu *InstancePtr, u8 *BufferPtr, u32 Length);

/*
 * Functions in xusbpsu_event.c
 */
void XUsbPsu_DisconnectIntr(struct XUsbPsu *InstancePtr);
void XUsbPsu_ResetIntr(struct XUsbPsu *InstancePtr);
void XUsbPsu_ConnDoneIntr(struct XUsbPsu *InstancePtr);
void XUsbPsu_LinkStsChangeIntr(struct XUsbPsu *InstancePtr,
			       u32 EvtInfo);
void XUsbPsu_EventHandler(struct XUsbPsu *InstancePtr,
			  const union XUsbPsu_Event *Event);
void XUsbPsu_Ep0Intr(struct XUsbPsu *InstancePtr,
		     const struct XUsbPsu_Event_Epevt *Event);

/*
 * Functions in xusbpsu_event.c
 */
void XUsbPsu_EpEvent(struct XUsbPsu *InstancePtr,
		     const struct XUsbPsu_Event_Epevt *Event);
void XUsbPsu_DeviceEvent(struct XUsbPsu *InstancePtr,
			 const struct XUsbPsu_Event_Devt *Event);
void XUsbPsu_EventBufferHandler(struct XUsbPsu *InstancePtr);

/*
 * Hibernation Functions
 */
#ifdef XUSBPSU_HIBERNATION_ENABLE
s32 XUsbPsu_InitHibernation(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_HibernationIntr(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_SetupScratchpad(struct XUsbPsu *InstancePtr, u8 *ScratchBuf);
s32 XUsbPsu_RestartEp(struct XUsbPsu *InstancePtr, u8 EpNum);
s32 XUsbPsu_RestoreEp0(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_RestoreEps(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_SendGadgetGenericCmd(struct XUsbPsu *InstancePtr, u32 cmd,
				 u32 param);
s32 XUsbPsu_HibernationStateIntr(struct XUsbPsu *InstancePtr);
s32 XUsbPsu_CoreRegRestore(struct XUsbPsu *InstancePtr);
#endif /* XUSBPSU_HIBERNATION_ENABLE */

#ifdef __cplusplus
}
#endif

#endif  /* End of protection macro. */
/** @endcond */
/** @} */
