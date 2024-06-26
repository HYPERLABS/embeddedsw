/******************************************************************************
* Copyright (C) 2016 - 2020 Xilinx, Inc. All rights reserved.
* Copyright 2023-2024 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xv_hdmitxss_coreinit.c
* @addtogroup v_hdmitxss Overview
* @{
* @details

* HDMI TX Subsystem Sub-Cores initialization
* The functions in this file provides an abstraction from the initialization
* sequence for included sub-cores. Subsystem is assigned an address and range
* on the axi-lite interface. This address space is condensed where-in each
* sub-core is at a fixed offset from the subsystem base address. For processor
* to be able to access the sub-core this offset needs to be transalted into a
* absolute address within the subsystems addressable range
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00         10/07/15 Initial release.
* 1.1   yh     20/01/16 Added remapper support
* 1.2   MG     03/02/16 Added HDCP support
* 1.3   MH     23/04/16 VTC driver has been updated to avoid processor
*                       exceptions. Workarounds have been removed.
* 1.4   MH     23/06/16 Added HDCP repeater support.
* 1.5   YH     18/07/16 Replace xil_printf with xdbg_printf
* 1.6   YH     25/07/16 Used UINTPTR instead of u32 for BaseAddr,HighAddr,Offset
*                       AbsAddr
* 1.7   MH     08/08/16 Updates to optimize out HDCP when excluded.
* 1.8   YH     17/08/16 Added Event Log
* 1.9   YH     14/11/16 Remove Remapper APIs as remapper feature is moved to
*                       video bridge and controlled by HDMI core
* 1.10  MMO    03/01/17 Remove XV_HdmiTxSs_ComputeSubcoreAbsAddr API, as it
*                            handles in the "_g" TCL generation
*                       Re-align coding style to ensure, 80 characters per row
* 3.2   MH     04/24/17 Set default HDMI mode in XV_HdmiTxSs_SubcoreInitHdmiTx
* 3.3   MH     07/17/17 Set HDCP 2.2 polling value to 10 ms.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xv_hdmitxss_coreinit.h"

/************************** Constant Definitions *****************************/

/************************** Function Prototypes ******************************/
#ifdef USE_HDCP_TX
static int XV_HdmiTxSs_DdcReadHandler(u8 DeviceAddress,
    u16 ByteCount, u8* BufferPtr, u8 Stop, void *RefPtr);
static int XV_HdmiTxSs_DdcWriteHandler(u8 DeviceAddress,
    u16 ByteCount, u8* BufferPtr, u8 Stop, void *RefPtr);
#endif

/*****************************************************************************/
/**
* This function initializes the included sub-core to it's static configuration
*
* @param  HdmiTxSsPtr is a pointer to the Subsystem instance to be worked on.
*
* @return XST_SUCCESS/XST_FAILURE
*
******************************************************************************/
int XV_HdmiTxSs_SubcoreInitHdmiTx(XV_HdmiTxSs *HdmiTxSsPtr)
{
  int Status;
  XV_HdmiTx_Config *ConfigPtr;

  if (HdmiTxSsPtr->HdmiTxPtr) {
    /* Get core configuration */
#ifdef XV_HDMITXSS_LOG_ENABLE
    XV_HdmiTxSs_LogWrite(HdmiTxSsPtr, XV_HDMITXSS_LOG_EVT_HDMITX_INIT, 0);
#endif
#ifndef SDT
    ConfigPtr  = XV_HdmiTx_LookupConfig(HdmiTxSsPtr->Config.HdmiTx.DeviceId);
#else
	ConfigPtr  = XV_HdmiTx_LookupConfig(HdmiTxSsPtr->Config.HdmiTx.AbsAddr);
#endif
    if (ConfigPtr == NULL) {
      xdbg_printf(XDBG_DEBUG_GENERAL,
                  "HDMITXSS ERR:: HDMI TX device not found\r\n");
      return(XST_FAILURE);
    }

#ifdef SDT
	HdmiTxSsPtr->Config.HdmiTx.AbsAddr += HdmiTxSsPtr->Config.BaseAddress;
	ConfigPtr->BaseAddress += HdmiTxSsPtr->Config.BaseAddress;
#endif
    /* Initialize core */
    Status = XV_HdmiTx_CfgInitialize(HdmiTxSsPtr->HdmiTxPtr,
                                    ConfigPtr,
                                    HdmiTxSsPtr->Config.HdmiTx.AbsAddr);

    if (Status != XST_SUCCESS) {
      xdbg_printf(XDBG_DEBUG_GENERAL,
                  "HDMITXSS ERR:: HDMI TX Initialization failed\r\n");
      return(XST_FAILURE);
    }

    /* Set default transmitter mode to HDMI */
    XV_HdmiTxSS_SetHdmiMode(HdmiTxSsPtr);
  }
  return(XST_SUCCESS);
}

/*****************************************************************************/
/**
* This function initializes the included sub-core to it's static configuration
*
* @param  HdmiTxSsPtr is a pointer to the Subsystem instance to be worked on.
*
* @return XST_SUCCESS/XST_FAILURE
*
******************************************************************************/
int XV_HdmiTxSs_SubcoreInitVtc(XV_HdmiTxSs *HdmiTxSsPtr)
{
  int Status;
  XVtc_Config *ConfigPtr;

  if (HdmiTxSsPtr->VtcPtr) {
    /* Get core configuration */
#ifdef XV_HDMITXSS_LOG_ENABLE
    XV_HdmiTxSs_LogWrite(HdmiTxSsPtr, XV_HDMITXSS_LOG_EVT_VTC_INIT, 0);
#endif
#ifndef SDT
    ConfigPtr  = XVtc_LookupConfig(HdmiTxSsPtr->Config.Vtc.DeviceId);
#else
	  ConfigPtr  = XVtc_LookupConfig(HdmiTxSsPtr->Config.Vtc.AbsAddr);
#endif
    if (ConfigPtr == NULL) {
      xdbg_printf(XDBG_DEBUG_GENERAL,"HDMITXSS ERR:: VTC device not found\r\n");
      return(XST_FAILURE);
    }

#ifdef SDT
	HdmiTxSsPtr->Config.Vtc.AbsAddr += HdmiTxSsPtr->Config.BaseAddress;
	ConfigPtr->BaseAddress += HdmiTxSsPtr->Config.BaseAddress;
#endif

    /* Initialize core */
    Status = XVtc_CfgInitialize(HdmiTxSsPtr->VtcPtr,
                                ConfigPtr,
                                HdmiTxSsPtr->Config.Vtc.AbsAddr);

    if (Status != XST_SUCCESS) {
      xdbg_printf(XDBG_DEBUG_GENERAL,
                  "HDMITXSS ERR:: VTC Initialization failed\r\n");
      return(XST_FAILURE);
    }
  }
  return(XST_SUCCESS);
}

#ifdef XPAR_XHDCP_NUM_INSTANCES
/*****************************************************************************/
/**
* This function initializes the included sub-core to it's static configuration
*
* @param  HdmiTxSsPtr is a pointer to the Subsystem instance to be worked on.
*
* @return XST_SUCCESS/XST_FAILURE
*
******************************************************************************/
int XV_HdmiTxSs_SubcoreInitHdcpTimer(XV_HdmiTxSs *HdmiTxSsPtr)
{
  int Status;
  XTmrCtr_Config *ConfigPtr;

  if (HdmiTxSsPtr->HdcpTimerPtr) {
    /* Get core configuration */
#ifdef XV_HDMITXSS_LOG_ENABLE
    XV_HdmiTxSs_LogWrite(HdmiTxSsPtr, XV_HDMITXSS_LOG_EVT_HDCPTIMER_INIT, 0);
#endif
#ifndef SDT
    ConfigPtr  = XTmrCtr_LookupConfig(HdmiTxSsPtr->Config.HdcpTimer.DeviceId);
#else
	ConfigPtr  = XTmrCtr_LookupConfig(HdmiTxSsPtr->Config.HdcpTimer.AbsAddr);
#endif
    if (ConfigPtr == NULL) {
      xdbg_printf(XDBG_DEBUG_GENERAL,
                  "HDMITXSS ERR:: AXIS Timer device not found\r\n");
      return(XST_FAILURE);
    }

#ifdef SDT
	HdmiTxSsPtr->Config.HdcpTimer.AbsAddr +=  HdmiTxSsPtr->Config.BaseAddress;
	ConfigPtr->BaseAddress += HdmiTxSsPtr->Config.BaseAddress;
#endif
    /* Setup the instance */
    memset(HdmiTxSsPtr->HdcpTimerPtr, 0, sizeof(XTmrCtr));

    /* Initialize core */
    XTmrCtr_CfgInitialize(HdmiTxSsPtr->HdcpTimerPtr,
                          ConfigPtr,
                          HdmiTxSsPtr->Config.HdcpTimer.AbsAddr);

    Status = XTmrCtr_InitHw(HdmiTxSsPtr->HdcpTimerPtr);

    /* Set Timer Counter instance in HDCP to the generic Hdcp1xRef
     * that will be used in callbacks */
    HdmiTxSsPtr->Hdcp14Ptr->Hdcp1xRef = (void *)HdmiTxSsPtr->HdcpTimerPtr;

    /* Initialize the hdcp timer functions */
    XHdcp1x_SetTimerStart(HdmiTxSsPtr->Hdcp14Ptr,
      &XV_HdmiTxSs_HdcpTimerStart);
    XHdcp1x_SetTimerStop(HdmiTxSsPtr->Hdcp14Ptr,
      &XV_HdmiTxSs_HdcpTimerStop);
    XHdcp1x_SetTimerDelay(HdmiTxSsPtr->Hdcp14Ptr,
      &XV_HdmiTxSs_HdcpTimerBusyDelay);

    if (Status != XST_SUCCESS) {
      xdbg_printf(XDBG_DEBUG_GENERAL,
                  "HDMITXSS ERR:: AXI Timer Initialization failed\r\n");
      return(XST_FAILURE);
    }
  }
  return(XST_SUCCESS);
}

/*****************************************************************************/
/**
* This function initializes the included sub-core to it's static configuration
*
* @param  HdmiTxSsPtr is a pointer to the Subsystem instance to be worked on.
*
* @return XST_SUCCESS/XST_FAILURE
*
******************************************************************************/
int XV_HdmiTxSs_SubcoreInitHdcp14(XV_HdmiTxSs *HdmiTxSsPtr)
{
  int Status;
  XHdcp1x_Config *ConfigPtr;

  /* Is the HDCP 1.4 TX present? */
  if (HdmiTxSsPtr->Hdcp14Ptr) {

    /* Is the key loaded? */
    if (HdmiTxSsPtr->Hdcp14KeyPtr) {

      /* Get core configuration */
#ifdef XV_HDMITXSS_LOG_ENABLE
      XV_HdmiTxSs_LogWrite(HdmiTxSsPtr, XV_HDMITXSS_LOG_EVT_HDCP14_INIT, 0);
#endif
#ifndef SDT
      ConfigPtr  = XHdcp1x_LookupConfig(HdmiTxSsPtr->Config.Hdcp14.DeviceId);
#else
	ConfigPtr  = XHdcp1x_LookupConfig(HdmiTxSsPtr->Config.Hdcp14.AbsAddr);
#endif
      if (ConfigPtr == NULL){
        xdbg_printf(XDBG_DEBUG_GENERAL,
                    "HDMITXSS ERR:: HDCP 1.4 device not found\r\n");
        return(XST_FAILURE);
      }
#ifdef SDT
	HdmiTxSsPtr->Config.Hdcp14.AbsAddr +=  HdmiTxSsPtr->Config.BaseAddress;
	ConfigPtr->BaseAddress += HdmiTxSsPtr->Config.BaseAddress;
#endif
      /* Initialize core */
      void *PhyIfPtr = HdmiTxSsPtr->HdmiTxPtr;

      Status = XHdcp1x_CfgInitialize(HdmiTxSsPtr->Hdcp14Ptr,
                                        ConfigPtr,
                                        PhyIfPtr,
                                        HdmiTxSsPtr->Config.Hdcp14.AbsAddr);

      /* Self-test the hdcp interface */
      if (XHdcp1x_SelfTest(HdmiTxSsPtr->Hdcp14Ptr) != XST_SUCCESS) {
          Status = XST_FAILURE;
      }

      /* Set-up the DDC Handlers */
      XHdcp1x_SetCallback(HdmiTxSsPtr->Hdcp14Ptr,
                          XHDCP1X_HANDLER_DDC_WRITE,
                          (void *)XV_HdmiTxSs_DdcWriteHandler,
						  (void *)HdmiTxSsPtr->HdmiTxPtr);

      XHdcp1x_SetCallback(HdmiTxSsPtr->Hdcp14Ptr,
                          XHDCP1X_HANDLER_DDC_READ,
						  (void *)XV_HdmiTxSs_DdcReadHandler,
						  (void *)HdmiTxSsPtr->HdmiTxPtr);

      if (Status != XST_SUCCESS) {
        xdbg_printf(XDBG_DEBUG_GENERAL,
                    "HDMITXSS ERR:: HDCP 1.4 Initialization failed\r\n");
        return(XST_FAILURE);
      }

      /* Key select */
      XHdcp1x_SetKeySelect(HdmiTxSsPtr->Hdcp14Ptr, XV_HDMITXSS_HDCP_KEYSEL);

      /* Load SRM */


      /* Disable HDCP 1.4 repeater */
      HdmiTxSsPtr->Hdcp14Ptr->IsRepeater = 0;
    }
  }
  return(XST_SUCCESS);
}
#endif

#ifdef XPAR_XHDCP22_TX_NUM_INSTANCES
/*****************************************************************************/
/**
* This function initializes the included sub-core to it's static configuration
*
* @param  HdmiTxSsPtr is a pointer to the Subsystem instance to be worked on.
*
* @return XST_SUCCESS/XST_FAILURE
*
******************************************************************************/
int XV_HdmiTxSs_SubcoreInitHdcp22(XV_HdmiTxSs *HdmiTxSsPtr)
{
  int Status;
  XHdcp22_Tx_Config *Hdcp22TxConfig;

  /* Is the HDCP 2.2 TX present? */
  if (HdmiTxSsPtr->Hdcp22Ptr) {

    /* Is the key loaded? */
    if (HdmiTxSsPtr->Hdcp22Lc128Ptr && HdmiTxSsPtr->Hdcp22SrmPtr) {

      /* Get core configuration */
#ifdef XV_HDMITXSS_LOG_ENABLE
      XV_HdmiTxSs_LogWrite(HdmiTxSsPtr, XV_HDMITXSS_LOG_EVT_HDCP22_INIT, 0);
#endif
#ifndef SDT
      /* Initialize HDCP 2.2 TX */
      Hdcp22TxConfig =
                    XHdcp22Tx_LookupConfig(HdmiTxSsPtr->Config.Hdcp22.DeviceId);
#else
	/* Initialize HDCP 2.2 TX */
	Hdcp22TxConfig =
		XHdcp22Tx_LookupConfig(HdmiTxSsPtr->Config.Hdcp22.AbsAddr);
#endif
#ifdef SDT
	HdmiTxSsPtr->Config.Hdcp22.AbsAddr +=  HdmiTxSsPtr->Config.BaseAddress;
	Hdcp22TxConfig->BaseAddress += HdmiTxSsPtr->Config.BaseAddress;
#endif
      if (Hdcp22TxConfig == NULL) {
        xdbg_printf(XDBG_DEBUG_GENERAL,
                    "HDMITXSS ERR:: HDCP 2.2 device not found\r\n");
        return XST_FAILURE;
      }

      Status = XHdcp22Tx_CfgInitialize(HdmiTxSsPtr->Hdcp22Ptr,
                                       Hdcp22TxConfig,
                                       HdmiTxSsPtr->Config.Hdcp22.AbsAddr);
      if (Status != XST_SUCCESS) {
        xdbg_printf(XDBG_DEBUG_GENERAL,
                    "HDMITXSS ERR:: HDCP 2.2 Initialization failed\r\n");
        return Status;
      }

      /* Set-up the DDC Handlers */
      XHdcp22Tx_SetCallback(HdmiTxSsPtr->Hdcp22Ptr,
                            XHDCP22_TX_HANDLER_DDC_WRITE,
							(void *)XV_HdmiTxSs_DdcWriteHandler,
							(void *)HdmiTxSsPtr->HdmiTxPtr);

      XHdcp22Tx_SetCallback(HdmiTxSsPtr->Hdcp22Ptr,
                            XHDCP22_TX_HANDLER_DDC_READ,
							(void *)XV_HdmiTxSs_DdcReadHandler,
							(void *)HdmiTxSsPtr->HdmiTxPtr);

      /* Set polling value */
      XHdcp22Tx_SetMessagePollingValue(HdmiTxSsPtr->Hdcp22Ptr, 10);

      XHdcp22Tx_LogReset(HdmiTxSsPtr->Hdcp22Ptr, FALSE);

      /* Load key */
      XHdcp22Tx_LoadLc128(HdmiTxSsPtr->Hdcp22Ptr, HdmiTxSsPtr->Hdcp22Lc128Ptr);

      /* Load SRM */
      Status = XHdcp22Tx_LoadRevocationTable(HdmiTxSsPtr->Hdcp22Ptr,
                                             HdmiTxSsPtr->Hdcp22SrmPtr);
      if (Status != XST_SUCCESS) {
        xdbg_printf(XDBG_DEBUG_GENERAL,
                    "HDMITXSS ERR:: HDCP 2.2 failed to load SRM\r\n");
        return Status;
      }

      /* Clear the event queue */
      XV_HdmiTxSs_HdcpClearEvents(HdmiTxSsPtr);
    }
  }

  return (XST_SUCCESS);
}
#endif


#ifdef USE_HDCP_TX
/*****************************************************************************/
/**
 *
 * This is the DDC read handler for the TX.
 *
 * @param DeviceAddress is the 7-bit I2C slave address
 *
 * @param ByteCount is the number of bytes to read
 *
 * @param BufferPtr is a pointer to the buffer where
 *        the read data is written to.
 *
 * @param Stop indicates if a I2C stop condition is generated
 *        at the end of the burst.
 *
 * @param RefPtr is a callback reference to the HDMI TX instance
 *
 * @return
 *  - XST_SUCCESS if action was successful
 *  - XST_FAILURE if action was not successful
 *
 ******************************************************************************/
static int XV_HdmiTxSs_DdcReadHandler(u8 DeviceAddress,
    u16 ByteCount, u8* BufferPtr, u8 Stop, void *RefPtr)
{
  XV_HdmiTx *InstancePtr = (XV_HdmiTx *)RefPtr;
  return XV_HdmiTx_DdcRead(InstancePtr,
                           DeviceAddress,
                           ByteCount,
                           BufferPtr,
                           Stop);
}

/*****************************************************************************/
/**
 *
 * This is the DDC write handler for the TX.
 *
 * @param DeviceAddress is the 7-bit I2C slave address
 *
 * @param ByteCount is the number of bytes to write
 *
 * @param BufferPtr is a pointer to the buffer containing
 *        the data to be written.
 *
 * @param Stop indicates if a I2C stop condition is generated
 *        at the end of the burst.
 *
 * @param RefPtr is a callback reference to the HDMI TX instance
 *
 * @return
 *  - XST_SUCCESS if action was successful
 *  - XST_FAILURE if action was not successful
 *
 ******************************************************************************/
static int XV_HdmiTxSs_DdcWriteHandler(u8 DeviceAddress,
    u16 ByteCount, u8* BufferPtr, u8 Stop, void *RefPtr)
{
  XV_HdmiTx *InstancePtr = (XV_HdmiTx *)RefPtr;
  return XV_HdmiTx_DdcWrite(InstancePtr,
                            DeviceAddress,
                            ByteCount,
                            BufferPtr,
                            Stop);
}
#endif

/** @} */
