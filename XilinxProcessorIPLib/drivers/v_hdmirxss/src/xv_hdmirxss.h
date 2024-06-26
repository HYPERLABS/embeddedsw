/******************************************************************************
* Copyright (C) 2016 - 2020 Xilinx, Inc. All rights reserved.
* Copyright 2023-2024 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xv_hdmirxss.h
*
* This is main header file of the Xilinx HDMI RX Subsystem driver
*
* <b>HDMI RX Subsystem Overview</b>
*
* Video Subsystem is a collection of IP cores bounded together by software
* to provide an abstract view of the processing pipe. It hides all the
* complexities of programming the underlying cores from end user.
*
* <b>Subsystem Driver Features</b>
*
* Video Subsystem supports following features
*   - AXI Stream Input/Output interface
*   - 1, 2 or 4 pixel-wide video interface
*   - 8/10/12/16 bits per component
*   - RGB & YCbCr color space
*   - Up to 4k2k 60Hz resolution at both Input and Output interface
*   - Interlaced input support (1080i 50Hz/60Hz)
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00         10/07/15 Initial release.
* 1.1   yh     20/01/16 Added remapper support
* 1.2   yh     01/02/16 Added set_ppc api
* 1.3   MG     03/02/16 Added HDCP support
* 1.4   MH     03/15/16 Added HDCP connect event.
*                       Added HDCP authenticated callback support.
* 1.5   YH     17/03/16 Remove xintc.h as it is processor dependent
* 1.6   MH     23/06/16 Added HDCP repeater support.
* 1.7   YH     25/07/16 Used UINTPTR instead of u32 for BaseAddress
* 1.8   MH     26/07/16 Updates for automatic protocol switching
* 1.9   MH     05/08/16 Updates to optimize out HDCP when excluded
* 1.10  YH     18/08/16 Combine Report function into one ReportInfo
* 1.11  YH     14/11/16 Added API to enable/disable YUV420/Pixel Drop Mode
*                       for video bridge
* 1.15  YH     14/11/16 Remove Remapper APIs as remapper feature is moved to
*                       video bridge and controlled by HDMI core
* 1.16  MMO    03/01/17 Add compiler option(XV_HDMIRXSS_LOG_ENABLE) to enable
*                         Log
*						Re-order the enumaration and data structure
*                       Move HDCP local API into _hdcp.h
*       ms     03/17/17 Added readme.txt file in examples folder for doxygen
*                       generation.
*       ms     04/10/17 Modified filename tag in examples to include them in
*                        doxygen.
* 1.4   YH     07/07/17 Add new log type XV_HDMIRXSS_LOG_EVT_SETSTREAM_ERR
*       MH     09/08/17 Add function XV_HdmiRxSs_HdcpSetCapability
*       YH     06/10/17 Add function XV_HdmiRxSs_GetAudioFormat
* 5.0   YH     16/11/17 Added dedicated reset for each clock domain
*              16/11/17 Added bridge overflow interrupt
*       EB     16/01/18 Added InfoFrame data structure to XV_HdmiRxSs
*                       Added XV_HDMIRXSS_LOG_EVT_PIX_REPEAT_ERR log event
*                       Added functions XV_HdmiRxSs_GetAviInfoframe,
*                           XV_HdmiRxSs_GetGCP, XV_HdmiRxSs_GetAudioInfoframe,
*                           XV_HdmiRxSs_GetVSIF
*       SM     28/02/18 Added XV_HdmiRxSS_SetAppVersion API and AppMajVer and
*                           AppMinVer version number in XV_HdmiRxSs structure
* 5.2   YB     08/14/18 Added dedicated callbacks for HDCP 1.4 and HDCP 2.2
*                           protocol events.
*       EB     03/08/18 Added function XV_HdmiRxSs_AudioMute
*                       Added TMDS Clock Ratio callback support
*       YB     17/08/18 Marked XV_HDMIRXSS_HDCP_1_PROT_EVT and
*                           XV_HDMIRXSS_HDCP_2_PROT_EVT as deprecated.
* 5.40  EB     06/08/19 Added Vic and Video Timing mismatch callback support
* </pre>
*
******************************************************************************/

#ifndef HDMIRXSS_H /**< prevent circular inclusions by using protection macros*/
#define HDMIRXSS_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xstatus.h"
#include "xv_hdmirx.h"
#include "xv_hdmic_vsif.h"
#if !defined(XV_CONFIG_LOG_VHDMIRXSS_DISABLE) && \
                                             !defined(XV_CONFIG_LOG_DISABLE_ALL)
#define XV_HDMIRXSS_LOG_ENABLE
#endif

#ifdef SDT
#if XPAR_XHDCP1X_NUM_INSTANCES
#define XPAR_XHDCP_NUM_INSTANCES XPAR_XHDCP1X_NUM_INSTANCES
#endif
#endif

#if defined(XPAR_XHDCP_NUM_INSTANCES) || defined(XPAR_XHDCP22_RX_NUM_INSTANCES)
#define USE_HDCP_RX
#define USE_HDCP_14_PROT_EVT_ENUM
#pragma message ("'XV_HDMIRXSS_HDCP_1_PROT_EVT' event is deprecated")
#define USE_HDCP_22_PROT_EVT_ENUM
#pragma message ("'XV_HDMIRXSS_HDCP_2_PROT_EVT' event is deprecated")
#define XV_HDMIRXSS_HDCP_KEYSEL 0x00u
#define XV_HDMIRXSS_HDCP_MAX_QUEUE_SIZE 16
#endif

#ifdef XPAR_XHDCP_NUM_INSTANCES
#include "xtmrctr.h"
#include "xhdcp1x.h"
#endif

#ifdef XPAR_XHDCP22_RX_NUM_INSTANCES
#include "xhdcp22_rx.h"
#endif
/****************************** Type Definitions ******************************/
/** @name Handler Types
* @{
*/

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifdef XV_HDMIRXSS_LOG_ENABLE
typedef enum {
	XV_HDMIRXSS_LOG_EVT_NONE = 1,		/**< Log event none. */
	XV_HDMIRXSS_LOG_EVT_HDMIRX_INIT,	/**< Log event HDMIRX Init. */
	XV_HDMIRXSS_LOG_EVT_VTC_INIT,	    /**< Log event VTC Init. */
	XV_HDMIRXSS_LOG_EVT_HDCPTIMER_INIT,	/**< Log event HDCP Timer Init */
	XV_HDMIRXSS_LOG_EVT_HDCP14_INIT,	/**< Log event HDCP 14 Init. */
	XV_HDMIRXSS_LOG_EVT_HDCP22_INIT,	/**< Log event HDCP 22 Init. */
	XV_HDMIRXSS_LOG_EVT_START,	        /**< Log event HDMIRXSS Start. */
	XV_HDMIRXSS_LOG_EVT_STOP,	        /**< Log event HDMIRXSS Stop. */
	XV_HDMIRXSS_LOG_EVT_RESET,	        /**< Log event HDMIRXSS Reset. */
	XV_HDMIRXSS_LOG_EVT_CONNECT,        /**< Log event Cable connect. */
	XV_HDMIRXSS_LOG_EVT_DISCONNECT,	    /**< Log event Cable disconnect. */
	XV_HDMIRXSS_LOG_EVT_LINKSTATUS,     /**< Log event Link Status Error. */
	XV_HDMIRXSS_LOG_EVT_STREAMUP,	    /**< Log event Stream Up. */
	XV_HDMIRXSS_LOG_EVT_STREAMDOWN,	    /**< Log event Stream Down. */
	XV_HDMIRXSS_LOG_EVT_STREAMINIT,	    /**< Log event Stream Init. */
	XV_HDMIRXSS_LOG_EVT_SETSTREAM,      /**< Log event HDMIRXSS Setstream. */
	XV_HDMIRXSS_LOG_EVT_SETSTREAM_ERR,  /**< Log event HDMIRXSS Setstream Err. */
	XV_HDMIRXSS_LOG_EVT_REFCLOCKCHANGE, /**< Log event TMDS Ref clock change. */
	XV_HDMIRXSS_LOG_EVT_HDCP14,             /**< Log event Enable HDCP 1.4. */
	XV_HDMIRXSS_LOG_EVT_HDCP22,             /**< Log event Enable HDCP 2.2. */
	XV_HDMIRXSS_LOG_EVT_HDMIMODE,           /**< Log event HDMI Mode change. */
	XV_HDMIRXSS_LOG_EVT_DVIMODE,            /**< Log event HDMI Mode change. */
	XV_HDMIRXSS_LOG_EVT_SYNCLOSS,           /**< Log event Sync Loss detected. */
	XV_HDMIRXSS_LOG_EVT_PIX_REPEAT_ERR,		/**< Log event Unsupported Pixel Repetition. */
	XV_HDMIRXSS_LOG_EVT_SYNCEST,            /**< Log event Sync Loss detected. */
	XV_HDMIRXSS_LOG_EVT_VICERROR,           /**< Log event vic error detected. */
	XV_HDMIRXSS_LOG_EVT_DUMMY               /**< Dummy Event should be last */
} XV_HdmiRxSs_LogEvent;

/**
 * This typedef contains the logging mechanism for debug.
 */
typedef struct {
	u16 DataBuffer[256];		/**< Log buffer with event data. */
	u8 HeadIndex;			/**< Index of the head entry of the
						Event/DataBuffer. */
	u8 TailIndex;			/**< Index of the tail entry of the
						Event/DataBuffer. */
} XV_HdmiRxSs_Log;
#endif

#ifdef USE_HDCP_RX
/**
* These constants are used to identify fields inside the topology structure
*/
typedef enum {
  XV_HDMIRXSS_HDCP_TOPOLOGY_DEPTH,
  XV_HDMIRXSS_HDCP_TOPOLOGY_DEVICECNT,
  XV_HDMIRXSS_HDCP_TOPOLOGY_MAXDEVSEXCEEDED,
  XV_HDMIRXSS_HDCP_TOPOLOGY_MAXCASCADEEXCEEDED,
  XV_HDMIRXSS_HDCP_TOPOLOGY_HDCP2LEGACYDEVICEDOWNSTREAM,
  XV_HDMIRXSS_HDCP_TOPOLOGY_HDCP1DEVICEDOWNSTREAM,
  XV_HDMIRXSS_HDCP_TOPOLOGY_INVALID
} XV_HdmiRxSs_HdcpTopologyField;

/**
* These constants specify the HDCP Events
*/
typedef enum
{
  XV_HDMIRXSS_HDCP_NO_EVT,
  XV_HDMIRXSS_HDCP_STREAMUP_EVT,
  XV_HDMIRXSS_HDCP_STREAMDOWN_EVT,
  XV_HDMIRXSS_HDCP_CONNECT_EVT,
  XV_HDMIRXSS_HDCP_DISCONNECT_EVT,
#ifdef USE_HDCP_14_PROT_EVT_ENUM
  XV_HDMIRXSS_HDCP_1_PROT_EVT,
#endif
#ifdef USE_HDCP_22_PROT_EVT_ENUM
  XV_HDMIRXSS_HDCP_2_PROT_EVT,
#endif
  XV_HDMIRXSS_HDCP_DVI_MODE_EVT,
  XV_HDMIRXSS_HDCP_HDMI_MODE_EVT,
  XV_HDMIRXSS_HDCP_SYNC_LOSS_EVT,
  XV_HDMIRXSS_HDCP_SYNC_EST_EVT,
  XV_HDMIRXSS_HDCP_INVALID_EVT
} XV_HdmiRxSs_HdcpEvent;

/**
* These constants specify the HDCP key types
*/
typedef enum
{
  XV_HDMIRXSS_KEY_HDCP22_LC128,     /**< HDCP 2.2 LC128 */
  XV_HDMIRXSS_KEY_HDCP22_PRIVATE,   /**< HDCP 2.2 Private */
  XV_HDMIRXSS_KEY_HDCP14,           /**< HDCP 1.4 Key */
} XV_HdmiRxSs_HdcpKeyType;

/**
* These constants specify HDCP repeater content stream management type
*/
typedef enum
{
  XV_HDMIRXSS_HDCP_STREAMTYPE_0, /**< HDCP Stream Type 0 */
  XV_HDMIRXSS_HDCP_STREAMTYPE_1  /**< HDCP Stream Type 1 */
} XV_HdmiRxSs_HdcpContentStreamType;

typedef struct
{
  XV_HdmiRxSs_HdcpEvent Queue[XV_HDMIRXSS_HDCP_MAX_QUEUE_SIZE]; /**< Data */
  u8                    Tail;      /**< Tail pointer */
  u8                    Head;      /**< Head pointer */
} XV_HdmiRxSs_HdcpEventQueue;
#endif

/**
* These constants specify the HDCP protection schemes
*/
typedef enum
{
  XV_HDMIRXSS_HDCP_NONE,       /**< No content protection */
  XV_HDMIRXSS_HDCP_14,         /**< HDCP 1.4 */
  XV_HDMIRXSS_HDCP_22,         /**< HDCP 2.2 */
  XV_HDMIRXSS_HDCP_BOTH,       /**< Both HDCP 1.4 and 2.2 */
  XV_HDMIRXSS_HDCP_NOUSERPREF,       /**< Control user selection.
				       * to be used only with API
				       * XV_HdmiRxSs_SetUserHdcpProtocol */
} XV_HdmiRxSs_HdcpProtocol;

/**
* These constants specify different types of handler and used to differentiate
* interrupt requests from peripheral.
*/
typedef enum {
  XV_HDMIRXSS_HANDLER_CONNECT = 1,                  /**< Handler for connect
                                                         event */
  XV_HDMIRXSS_HANDLER_BRDGOVERFLOW,                 /**< Handler for
                                                         bridge fifo overflow
                                                         event */
  XV_HDMIRXSS_HANDLER_AUX,                          /**< Handler for AUX
                                                         peripheral event */
  XV_HDMIRXSS_HANDLER_AUD,                          /**< Handler for AUD
                                                         peripheral event */
  XV_HDMIRXSS_HANDLER_LNKSTA,                       /**< Handler for LNKSTA
                                                         peripheral event */
  XV_HDMIRXSS_HANDLER_DDC,                          /**< Handler for DDC
                                                         peripheral event */
  XV_HDMIRXSS_HANDLER_STREAM_DOWN,                  /**< Handler for stream
                                                         down event */
  XV_HDMIRXSS_HANDLER_STREAM_INIT,                  /**< Handler for stream
                                                         init event */
  XV_HDMIRXSS_HANDLER_STREAM_UP,                    /**< Handler for stream up
                                                         event */
  XV_HDMIRXSS_HANDLER_HDCP,                         /**< Handler for HDCP 1.4
                                                         event */
  XV_HDMIRXSS_HANDLER_HDCP_AUTHENTICATED,           /**< Handler for HDCP
                                                         authenticated event */
  XV_HDMIRXSS_HANDLER_HDCP_UNAUTHENTICATED,         /**< Handler for HDCP
                                                         unauthenticated event*/
  XV_HDMIRXSS_HANDLER_HDCP_AUTHENTICATION_REQUEST,  /**< Handler for HDCP
                                                         authentication request
                                                         event */
  XV_HDMIRXSS_HANDLER_HDCP_STREAM_MANAGE_REQUEST,   /**< Handler for HDCP stream
                                                         manage request event */
  XV_HDMIRXSS_HANDLER_HDCP_TOPOLOGY_UPDATE,         /**< Handler for HDCP
                                                         topology update event*/
  XV_HDMIRXSS_HANDLER_HDCP_ENCRYPTION_UPDATE,       /**< Handler for HDCP
                                                         encryption status
                                                         update event */
  XV_HDMIRXSS_HANDLER_TMDS_CLK_RATIO,               /**< Handler type for
                                                         TMDS clock ratio
                                                         change */
  XV_HDMIRXSS_HANDLER_VIC_ERROR                     /**< Handler type for
                                                         VIC error change */
} XV_HdmiRxSs_HandlerType;
/*@}*/

/**
 * Sub-Core Configuration Table
 */
typedef struct
{
  u16 IsPresent;  /**< Flag to indicate if sub-core is present in the design*/
#ifndef SDT
  u16 DeviceId;   /**< Device ID of the sub-core */
#endif
  UINTPTR AbsAddr; /**< Absolute Base Address of hte Sub-cores*/
}XV_HdmiRxSs_SubCore;

/**
 * Video Processing Subsystem configuration structure.
 * Each subsystem device should have a configuration structure associated
 * that defines the MAX supported sub-cores within subsystem
 */

typedef struct
{
#ifndef SDT
	u16 DeviceId;     /**< DeviceId is the unique ID  of the device */
#else
	char *Name;
#endif
  UINTPTR BaseAddress;  /**< BaseAddress is the physical base address of the
                        subsystem address range */
  UINTPTR HighAddress;  /**< HighAddress is the physical MAX address of the
                        subsystem address range */
  XVidC_PixelsPerClock Ppc;         /**< Supported Pixel per Clock */
  u8 MaxBitsPerPixel;               /**< Maximum  Supported Color Depth */
  XV_HdmiRxSs_SubCore HdcpTimer;    /**< Sub-core instance configuration */
  XV_HdmiRxSs_SubCore Hdcp14;       /**< Sub-core instance configuration */
  XV_HdmiRxSs_SubCore Hdcp22;       /**< Sub-core instance configuration */
  XV_HdmiRxSs_SubCore HdmiRx;       /**< Sub-core instance configuration */
#ifdef SDT
	u16 IntrId[5];	/**< Interrupt ID */
	UINTPTR IntrParent;	/**< Bit[0] Interrupt parent type Bit[64/32:1]
						Parent base address */
#endif
} XV_HdmiRxSs_Config;

/**
* Callback type for interrupt.
*
* @param  CallbackRef is a callback reference passed in by the upper
*   layer when setting the callback functions, and passed back to
*   the upper layer when the callback is invoked.
*
* @return None.
*
* @note   None.
*
*/
typedef void (*XV_HdmiRxSs_Callback)(void *CallbackRef);

/**
* The XVprocss driver instance data. The user is required to allocate a variable
* of this type for every XVprocss device in the system. A pointer to a variable
* of this type is then passed to the driver API functions.
*/
typedef struct
{
  XV_HdmiRxSs_Config Config;    /**< Hardware configuration */
  u32 IsReady;                  /**< Device and the driver instance are
                                     initialized */
  u8 AppMajVer;       /**< Major Version of application used by the driver */
  u8 AppMinVer;       /**< Minor Version of application used by the driver */

#ifdef XV_HDMIRXSS_LOG_ENABLE
  XV_HdmiRxSs_Log Log;				/**< A log of events. */
#endif

#ifdef XPAR_XHDCP_NUM_INSTANCES
  XTmrCtr *HdcpTimerPtr;           /**< handle to sub-core driver instance */
  XHdcp1x *Hdcp14Ptr;                /**< handle to sub-core driver instance */
#endif

#ifdef XPAR_XHDCP22_RX_NUM_INSTANCES
  XHdcp22_Rx  *Hdcp22Ptr;           /**< handle to sub-core driver instance */
#endif
  XV_HdmiRx *HdmiRxPtr;             /**< handle to sub-core driver instance */

  /*Callbacks */
  XV_HdmiRxSs_Callback ConnectCallback; /**< Callback for connect event */
  void *ConnectRef;     /**< To be passed to the connect callback */

  XV_HdmiRxSs_Callback BrdgOverflowCallback;  /**< Callback for Bridge overflow
                                           event */
  void *BrdgOverflowRef;         /**< To be passed to the Bridge overflow
                                    callback */

  XV_HdmiRxSs_Callback AuxCallback;     /**< Callback for AUX event */
  void *AuxRef;         /**< To be passed to the AUX callback */

  XV_HdmiRxSs_Callback AudCallback;     /**< Callback for AUD event */
  void *AudRef;         /**< To be passed to the AUD callback */

  XV_HdmiRxSs_Callback LnkStaCallback;  /**< Callback for LNKSTA event */
  void *LnkStaRef;      /**< To be passed to the LNKSTA callback */

  XV_HdmiRxSs_Callback DdcCallback;     /**< Callback for PDDC event */
  void *DdcRef;         /**< To be passed to the DDC callback */

  XV_HdmiRxSs_Callback StreamDownCallback;/**< Callback for stream down event */
  void *StreamDownRef;  /**< To be passed to the stream down callback */

  XV_HdmiRxSs_Callback StreamInitCallback;/**< Callback for stream init event */
  void *StreamInitRef;  /**< To be passed to the stream init callback */

  XV_HdmiRxSs_Callback StreamUpCallback; /**< Callback for stream up event */
  void *StreamUpRef;    /**< To be passed to the stream up callback */

  XV_HdmiRxSs_Callback HdcpCallback;    /**< Callback for HDCP 1.4 event */
  void *HdcpRef;        /**< To be passed to the hdcp callback */

  XV_HdmiRxSs_Callback TmdsClkRatioCallback;  /**< Callback for scdc TMDS clock
                                                   ratio change callback */
  void *TmdsClkRatioRef;/**< To be passed to the scdc tmds clock ratio change
                             callback */

  XV_HdmiRxSs_Callback VicErrorCallback;  /**< Callback for VIC error
                                               detection */
   void *VicErrorRef;   /**< To be passed to the VIC error callback */

  // Scratch pad
  u8 IsStreamConnected;         /**< HDMI RX Stream Connected */
  u8 IsStreamUp;                /**< HDMI RX Stream Up */
  u8 AudioChannels;             /**< Number of Audio Channels */
  int IsLinkStatusErrMax;       /**< Link Error Status Maxed */
  u8 *EdidPtr;                     /**< Default Edid Pointer */
  u16 EdidLength;               /**< Default Edid Length */
  u8 TMDSClockRatio;            /**< HDMI RX TMDS clock ratio */

  XHdmiC_AVI_InfoFrame AVIInfoframe;	/**< AVI InfoFrame */
  XHdmiC_GeneralControlPacket GCP;		/**< General Control Packet */
  XHdmiC_AudioInfoFrame AudioInfoframe;	/**< Audio InfoFrame */
  XHdmiC_VSIF VSIF;						/**< Vendor Specific InfoFrame */
  XHdmiC_DRMInfoFrame DrmInfoframe; /* HDR infoframe */

  XVidC_DelayHandler UserTimerWaitUs; /**< Custom user function for
                                           delay/sleep. */
  void *UserTimerPtr;                 /**< Pointer to a timer instance
                                           used by the custom user
                                           delay/sleep function. */

  XV_HdmiRxSs_HdcpProtocol      HdcpProtocol;   /**< HDCP protocol selected */
#ifdef USE_HDCP_RX
  /**< HDCP specific */
  u32                           HdcpIsReady;    /**< HDCP ready flag */
  XV_HdmiRxSs_HdcpProtocol	UserHdcpProt;    /**< User HDCP preference */
  XV_HdmiRxSs_HdcpEventQueue    HdcpEventQueue;         /**< HDCP event queue */
#endif
#ifdef XPAR_XHDCP22_RX_NUM_INSTANCES
  u8                            *Hdcp22Lc128Ptr;     /**< Pointer to HDCP 2.2
                                                         LC128 */
  u8                            *Hdcp22PrivateKeyPtr;   /**< Pointer to HDCP 2.2
                                                             Private key */
#endif
#ifdef XPAR_XHDCP_NUM_INSTANCES
  u8                            *Hdcp14KeyPtr;   /**< Pointer to HDCP 1.4 key */
#endif
} XV_HdmiRxSs;

/************************** Macros Definitions *******************************/
#ifdef USE_HDCP_RX
#define XV_HdmiRxSs_HdcpIsReady(InstancePtr) \
  (InstancePtr)->HdcpIsReady
#endif
/************************** Function Prototypes ******************************/
#ifndef SDT
XV_HdmiRxSs_Config* XV_HdmiRxSs_LookupConfig(u32 DeviceId);
#else
XV_HdmiRxSs_Config* XV_HdmiRxSs_LookupConfig(UINTPTR BaseAddress);
u32 XV_HdmiRxSs_GetDrvIndex(XV_HdmiRxSs *InstancePtr, UINTPTR BaseAddress);
#endif
void XV_HdmiRxSs_SetUserTimerHandler(XV_HdmiRxSs *InstancePtr,
        XVidC_DelayHandler CallbackFunc, void *CallbackRef);
void XV_HdmiRxSS_HdmiRxIntrHandler(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_CfgInitialize(XV_HdmiRxSs *InstancePtr,
    XV_HdmiRxSs_Config *CfgPtr,
    UINTPTR EffectiveAddr);
void XV_HdmiRxSs_Write_ScdcRegister(XV_HdmiRxSs *InstancePtr, u8 address, u8 data);
void XV_HdmiRxSS_SetAppVersion(XV_HdmiRxSs *InstancePtr, u8 maj, u8 min);
void XV_HdmiRxSs_Start(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_Stop(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_Reset(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_RXCore_VRST(XV_HdmiRxSs *InstancePtr, u8 Reset);
void XV_HdmiRxSs_RXCore_LRST(XV_HdmiRxSs *InstancePtr, u8 Reset);
void XV_HdmiRxSs_VRST(XV_HdmiRxSs *InstancePtr, u8 Reset);
void XV_HdmiRxSs_SYSRST(XV_HdmiRxSs *InstancePtr, u8 Reset);
int XV_HdmiRxSs_SetCallback(XV_HdmiRxSs *InstancePtr,
		XV_HdmiRxSs_HandlerType HandlerType,
		void *CallbackFunc,
		void *CallbackRef);
void XV_HdmiRxSs_SetEdidParam(XV_HdmiRxSs *InstancePtr, u8 *EdidDataPtr,
                                                                u16 Length);
void XV_HdmiRxSs_LoadDefaultEdid(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_LoadEdid(XV_HdmiRxSs *InstancePtr, u8 *EdidDataPtr,
                                                                u16 Length);
void XV_HdmiRxSs_SetHpd(XV_HdmiRxSs *InstancePtr, u8 Value);
void XV_HdmiRxSs_ToggleHpd(XV_HdmiRxSs *InstancePtr);
XHdmiC_Aux *XV_HdmiRxSs_GetAuxiliary(XV_HdmiRxSs *InstancePtr);
XHdmiC_AVI_InfoFrame *XV_HdmiRxSs_GetAviInfoframe(XV_HdmiRxSs *InstancePtr);
XHdmiC_AudioInfoFrame *XV_HdmiRxSs_GetAudioInfoframe(XV_HdmiRxSs *InstancePtr);
XHdmiC_DRMInfoFrame *XV_HdmiRxSs_GetDrmInfoframe(XV_HdmiRxSs *InstancePtr);
XHdmiC_VSIF *XV_HdmiRxSs_GetVSIF(XV_HdmiRxSs *InstancePtr);
XHdmiC_GeneralControlPacket *XV_HdmiRxSs_GetGCP(XV_HdmiRxSs *InstancePtr);
u32 XV_HdmiRxSs_SetStream(XV_HdmiRxSs *InstancePtr,
    u32 Clock,
    u32 LineRate);
XVidC_VideoStream *XV_HdmiRxSs_GetVideoStream(XV_HdmiRxSs *InstancePtr);
u8 XV_HdmiRxSs_GetVideoIDCode(XV_HdmiRxSs *InstancePtr);
u8 XV_HdmiRxSs_GetVideoStreamType(XV_HdmiRxSs *InstancePtr);
u8 XV_HdmiRxSs_GetVideoStreamScramblingFlag(XV_HdmiRxSs *InstancePtr);
u8 XV_HdmiRxSs_GetAudioChannels(XV_HdmiRxSs *InstancePtr);
XV_HdmiRx_AudioFormatType XV_HdmiRxSs_GetAudioFormat(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_RefClockChangeInit(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_ReportInfo(XV_HdmiRxSs *InstancePtr);
int  XV_HdmiRxSs_IsStreamUp(XV_HdmiRxSs *InstancePtr);
int  XV_HdmiRxSs_IsStreamConnected(XV_HdmiRxSs *InstancePtr);

void XV_HdmiRxSs_SetDefaultPpc(XV_HdmiRxSs *InstancePtr, u8 Id);
void XV_HdmiRxSs_SetPpc(XV_HdmiRxSs *InstancePtr, u8 Id, u8 Ppc);
void XV_HdmiRxSs_AudioMute(XV_HdmiRxSs *InstancePtr, u8 Enable);
int XV_HdmiRxSs_SetUserHdcpProtocol(XV_HdmiRxSs *InstancePtr,
		XV_HdmiRxSs_HdcpProtocol protocol);


#ifdef XV_HDMIRXSS_LOG_ENABLE
void XV_HdmiRxSs_LogReset(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_LogWrite(XV_HdmiRxSs *InstancePtr, XV_HdmiRxSs_LogEvent Evt, u8 Data);
u16 XV_HdmiRxSs_LogRead(XV_HdmiRxSs *InstancePtr);
#endif
void XV_HdmiRxSs_LogDisplay(XV_HdmiRxSs *InstancePtr);

#ifdef USE_HDCP_RX
void XV_HdmiRxSs_HdcpSetKey(XV_HdmiRxSs *InstancePtr, XV_HdmiRxSs_HdcpKeyType KeyType, u8 *KeyPtr);
int XV_HdmiRxSs_HdcpEnable(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpDisable(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpClearEvents(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpPushEvent(XV_HdmiRxSs *InstancePtr, XV_HdmiRxSs_HdcpEvent Event);
int XV_HdmiRxSs_HdcpPoll(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpSetProtocol(XV_HdmiRxSs *InstancePtr, XV_HdmiRxSs_HdcpProtocol Protocol);
int XV_HdmiRxSs_HdcpSetCapability(XV_HdmiRxSs *InstancePtr, XV_HdmiRxSs_HdcpProtocol Protocol);
XV_HdmiRxSs_HdcpProtocol XV_HdmiRxSs_HdcpGetProtocol(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsEnabled(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsAuthenticated(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsEncrypted(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsInProgress(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_HdcpInfo(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSs_HdcpSetInfoDetail(XV_HdmiRxSs *InstancePtr, u8 Verbose);
int XV_HdmiRxSs_HdcpSetTopology(XV_HdmiRxSs *InstancePtr, void *TopologyPtr);
int XV_HdmiRxSs_HdcpSetTopologyReceiverIdList(XV_HdmiRxSs *InstancePtr, u8 *ListPtr, u32 ListSize);
int XV_HdmiRxSs_HdcpSetTopologyField(XV_HdmiRxSs *InstancePtr,
      XV_HdmiRxSs_HdcpTopologyField Field, u32 Value);
int XV_HdmiRxSs_HdcpSetRepeater(XV_HdmiRxSs *InstancePtr, u8 Set);
int XV_HdmiRxSs_HdcpSetTopologyUpdate(XV_HdmiRxSs *InstancePtr);
XV_HdmiRxSs_HdcpContentStreamType XV_HdmiRxSs_HdcpGetContentStreamType(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsRepeater(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsInWaitforready(XV_HdmiRxSs *InstancePtr);
int XV_HdmiRxSs_HdcpIsInComputations(XV_HdmiRxSs *InstancePtr);

#ifdef XPAR_XHDCP_NUM_INSTANCES
int XV_HdmiRxSs_HdcpTimerStart(void *InstancePtr, u16 TimeoutInMs);
int XV_HdmiRxSs_HdcpTimerStop(void *InstancePtr);
int XV_HdmiRxSs_HdcpTimerBusyDelay(void *InstancePtr, u16 DelayInMs);
void XV_HdmiRxSS_HdcpIntrHandler(XV_HdmiRxSs *InstancePtr);
void XV_HdmiRxSS_HdcpTimerIntrHandler(XV_HdmiRxSs *InstancePtr);
#endif

#ifdef XPAR_XHDCP22_RX_NUM_INSTANCES
void XV_HdmiRxSS_Hdcp22TimerIntrHandler(XV_HdmiRxSs *InstancePtr);
#endif

#endif // USE_HDCP_RX

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
