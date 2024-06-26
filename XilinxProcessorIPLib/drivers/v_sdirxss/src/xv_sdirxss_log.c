/*******************************************************************************
* Copyright (C) 2017 - 2020 Xilinx, Inc. All rights reserved.
* Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
*******************************************************************************/

/******************************************************************************/
/**
 *
 * @file xv_sdirxss_log.c
 *
 *
 * @note	None.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -----------------------------------------------
 * 1.0   jsr  07/17/17 Initial release.
 * </pre>
 *
*******************************************************************************/

/******************************* Include Files ********************************/

#include "xv_sdirxss.h"

/**************************** Function Prototypes *****************************/

/**************************** Function Definitions ****************************/

/*****************************************************************************/
/**
* This function will reset the driver's logging mechanism.
*
* @param	InstancePtr is a pointer to the xv_sdirxss core instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XV_SdiRxSs_LogReset(XV_SdiRxSs *InstancePtr)
{
	/* Verify arguments. */
	Xil_AssertVoid(InstancePtr != NULL);

	InstancePtr->Log.HeadIndex = 0;
	InstancePtr->Log.TailIndex = 0;
}

/*****************************************************************************/
/**
* This function will insert an event in the driver's logginc mechanism.
*
* @param	InstancePtr is a pointer to the XV_SdiRxSs core instance.
* @param	Evt is the event type to log.
* @param	Data is the associated data for the event.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XV_SdiRxSs_LogWrite(XV_SdiRxSs *InstancePtr, XV_SdiRxSs_LogEvent Evt, u8 Data)
{
	/* Verify arguments. */
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(Evt <= (XV_SDIRXSS_LOG_EVT_DUMMY));
	Xil_AssertVoid(Data < 0xFF);

	/* Write data and event into log buffer */
	InstancePtr->Log.DataBuffer[InstancePtr->Log.HeadIndex] =
			(Data << 8) | Evt;

	/* Update head pointer if reached to end of the buffer */
	if (InstancePtr->Log.HeadIndex ==
			(u8)((sizeof(InstancePtr->Log.DataBuffer) / 2) - 1)) {
		/* Clear pointer */
		InstancePtr->Log.HeadIndex = 0;
	} else {
		/* Increment pointer */
		InstancePtr->Log.HeadIndex++;
	}

	/* Check tail pointer. When the two pointer are equal, then the buffer
	 * is full. In this case then increment the tail pointer as well to
	 * remove the oldest entry from the buffer. */
	if (InstancePtr->Log.TailIndex == InstancePtr->Log.HeadIndex) {
		if (InstancePtr->Log.TailIndex ==
			(u8)((sizeof(InstancePtr->Log.DataBuffer) / 2) - 1)) {
			InstancePtr->Log.TailIndex = 0;
		} else {
			InstancePtr->Log.TailIndex++;
		}
	}
}

/*****************************************************************************/
/**
* This function will read the last event from the log.
*
* @param	InstancePtr is a pointer to the XV_SdiRxSs core instance.
*
* @return	The log data.
*
* @note		None.
*
******************************************************************************/
u16 XV_SdiRxSs_LogRead(XV_SdiRxSs *InstancePtr)
{
	u16 Log;

	/* Verify argument. */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/* Check if there is any data in the log */
	if (InstancePtr->Log.TailIndex == InstancePtr->Log.HeadIndex) {
		Log = 0;
	} else {
		Log = InstancePtr->Log.DataBuffer[InstancePtr->Log.TailIndex];

		/* Increment tail pointer */
		if (InstancePtr->Log.TailIndex ==
			(u8)((sizeof(InstancePtr->Log.DataBuffer) / 2) - 1)) {
			InstancePtr->Log.TailIndex = 0;
		} else {
			InstancePtr->Log.TailIndex++;
		}
	}

	return Log;
}

/*****************************************************************************/
/**
* This function will print the entire log.
*
* @param	InstancePtr is a pointer to the XV_SdiRxSs core instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XV_SdiRxSs_LogDisplay(XV_SdiRxSs *InstancePtr)
{
	u16 Log;
	u8 Evt;
	u8 Data;

	/* Verify argument. */
	Xil_AssertVoid(InstancePtr != NULL);

	xil_printf("\r\n\n\nSDI RX log\r\n");
	xil_printf("------\r\n");

	/* Read log data */
	Log = XV_SdiRxSs_LogRead(InstancePtr);

	while (Log != 0) {
		/* Event */
		Evt = Log & 0xff;

		/* Data */
		Data = (Log >> 8) & 0xFF;
		Data = Data;

		switch (Evt) {
		case (XV_SDIRXSS_LOG_EVT_NONE):
			xil_printf("SDI RXSS log end\r\n-------\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_SDIRX_INIT):
			xil_printf("Initializing SDI RX core....\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_START):
			xil_printf("Start SDI RX Subsystem....\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_STOP):
			xil_printf("Stop SDI RX Subsystem....\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_RESET):
			xil_printf("Reset SDI RX Subsystem....\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_STREAMUP):
			xil_printf("RX Stream is Up\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_STREAMDOWN):
			xil_printf("RX Stream is Down\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_OVERFLOW):
			xil_printf("RX Stream is Overflowed\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_UNDERFLOW):
			xil_printf("RX Stream is Underflowed\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_STREAMSTART):
			xil_printf("RX Stream Start\r\n");
			break;
		case (XV_SDIRXSS_LOG_EVT_SETSTREAM):
			xil_printf("RX Set Stream, with TMDS (%0d)\r\n", Data);
			break;
		default:
			xil_printf("Unknown event\r\n");
			break;
		}

		/* Read log data */
		Log = XV_SdiRxSs_LogRead(InstancePtr);
	}
}
