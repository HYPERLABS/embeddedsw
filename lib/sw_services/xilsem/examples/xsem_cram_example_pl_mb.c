/******************************************************************************
* Copyright (c) 2022 Xilinx, Inc.  All rights reserved.
* Copyright (C) 2022 - 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* Copyright (C) 2023 - 2024 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/**
 * @file xsem_cram_example_pl_mb.c
 *
 * This file demonstrates on how to use XilSEM CRAM client interface on PL
 * Microblaze to send commands to PLM firmware and read responses from PLM.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who       Date        Changes
 * ----  ----   ----------  ---------------------------------------------------
 * 0.1   gupta   11/16/2022  Initial Creation
 * 0.2   anv     10/18/2023  Added macro protection to Enable Error Injection
 *                           Feature usage and Updated to get
 *                           XilSEM CRAM status prints and
 *                           Added Test print summary
 * 0.3   gam     01/02/2023  Fixed IPI interrupt ID for PL MB Versal Net,
 *                           changed Uncorrectable and CRC error data.
 *                           Moved Error injection sequence in the end of
 *                           main function to get clear prints.
 * </pre>
 *
 *****************************************************************************/
#include "xil_printf.h"
#include "sleep.h"
#include "xsem_client_api.h"
#include "xintc.h"

#define CRAM_STATUS_INIT_DONE_MASK	(0X00010000U)
#define CRAM_STATUS_COR_ERR_MASK	(0X00000800U)
#define CRAM_STATUS_CRC_ERR_MASK	(0X00000400U)
#define CRAM_STATUS_UNCOR_ERR_MASK	(0X00000200U)
#define CRAM_STATUS_ECC_COR_DONE_MASK	(0X00004000U)
/* Interrupt Controller device ID */
#define INTC_DEVICE_ID			XPAR_INTC_0_DEVICE_ID
/* IPI device ID to use for this test */
#define IPI_CHANNEL_ID			XPAR_XIPIPSU_0_DEVICE_ID
/* Destination IPI channel mask used for this test */
#define IPI_PMC_MASK			0x2U
/*
 * Interrupt Number of IPI whose interrupt output is connected to the input
 * of the Interrupt Controller
 */
#ifdef VERSAL_NET
	#define INTC_DEVICE_IPI_INT_ID	\
		XPAR_AXI_INTC_0_PSX_WIZARD_0_PSXL_0_PSX_PL_IRQ_LPD_IPI4_INTR
#else
	#define INTC_DEVICE_IPI_INT_ID	\
		XPAR_AXI_INTC_0_VERSAL_CIPS_0_PSPMC_0_PS_PL_IRQ_LPD_IPI_IPI1_INTR
#endif

static XIpiPsu IpiInst;
static XIntc InterruptController;

XSem_Notifier Notifier = {
        .Module = XSEM_NOTIFY_CRAM,
        .Event = XSEM_EVENT_CRAM_UNCOR_ECC_ERR | XSEM_EVENT_CRAM_CRC_ERR | \
		 XSEM_EVENT_CRAM_INT_ERR | XSEM_EVENT_CRAM_COR_ECC_ERR,
	.Flag = 1U,
};

/*Global variables to hold the event count when notified*/
u8 EventCnt_UnCorEcc = 0U;
u8 EventCnt_Crc = 0U;
u8 EventCnt_CorEcc = 0U;
u8 EventCnt_IntErr = 0U;

/*****************************************************************************
 *
 * @brief    	Function to initialize CRAM scan using Client IPI interface.
 *
 * @return    	XST_SUCCESS : upon successful initialization of CRAM
 *  		XST_FAILURE : any failure in initialization of CRAM.
 *
 ******************************************************************************/
XStatus XSem_CfrApiInitCram (void)
{
	XStatus Status = XST_FAILURE;
	XSemIpiResp IpiResp={0};

	Status = XSem_CmdCfrInit(&IpiInst, &IpiResp);
	if ((XST_SUCCESS == Status) && (CMD_ACK_CFR_INIT == IpiResp.RespMsg1) \
			&& (XST_SUCCESS == IpiResp.RespMsg2)) {
		xil_printf("[%s] Success: Init\n\r", __func__);
	} else {
		xil_printf("[%s] Error: Init Status 0x%x Ack 0x%x Ret 0x%x\n", \
				__func__, Status, IpiResp.RespMsg1, \
				IpiResp.RespMsg2);
		Status = XST_FAILURE;
	}

	return Status;
}

/*****************************************************************************
 *
 * @brief    	Function to start CRAM scan using Client IPI interface.
 *
 * @return    	XST_SUCCESS : upon successful completion of start scan.
 *   		XST_FAILURE : any failure in start scan.
 *
 ******************************************************************************/
XStatus XSem_CfrApiStartScan (void)
{
	XStatus Status = XST_FAILURE;
	XSemIpiResp IpiResp={0};

	Status = XSem_CmdCfrStartScan(&IpiInst, &IpiResp);
	if ((XST_SUCCESS == Status) && \
			(CMD_ACK_CFR_START_SCAN == IpiResp.RespMsg1) && \
			(XST_SUCCESS == IpiResp.RespMsg2)) {
		xil_printf("[%s] Success: Start\n\r", __func__);
	} else {
		xil_printf("[%s] Error: Start Status 0x%x Ack 0x%x Ret 0x%x\n",\
				__func__, Status, IpiResp.RespMsg1, \
				IpiResp.RespMsg2);
		Status = XST_FAILURE;
	}

	return Status;
}

/*****************************************************************************
 *
 * @brief    	Function to stop CRAM scan using Client IPI interface.
 *
 * @return    	XST_SUCCESS : upon successful completion of stop scan.
 *    		XST_FAILURE : any failure in stop scan.
 *
 ******************************************************************************/
XStatus XSem_CfrApiStopScan (void)
{
	XStatus Status = XST_FAILURE;
	XSemIpiResp IpiResp={0};

	Status = XSem_CmdCfrStopScan(&IpiInst, &IpiResp);
	if ((XST_SUCCESS == Status) && \
			(CMD_ACK_CFR_STOP_SCAN == IpiResp.RespMsg1) && \
			(XST_SUCCESS == IpiResp.RespMsg2)) {
		xil_printf("[%s] Success: Stop\n\r", __func__);
	} else {
		xil_printf("[%s] Error: Stop Status 0x%x Ack 0x%x Ret 0x%x\n", \
				__func__, Status, IpiResp.RespMsg1, \
				IpiResp.RespMsg2);
		Status = XST_FAILURE;
	}
	return Status;
}

#ifdef XILSEM_ERRINJ_ENABLE
/*****************************************************************************
 *
 * @brief    	Function to inject error in CFRAME using Client IPI interface.
 *
 * @param    	ErrData : Pointer to Error details
 *
 * @return    	XST_SUCCESS : upon successful error injection.
 *   		XST_FAILURE : Any failure IPI interface
 *     		Error Codes : Below error codes upon failure during injection
 *		0x00500000U : If CRAM Scan is not initialized
 * 		0X00002000U : On getting Cframe driver instance as Null
 * 		0x00800000U : If invalid row is passed as input
 * 		0x00E00000U : If invalid block type is passed as input
 * 		0x00A00000U : If invalid bit is passed as input
 * 		0x00B00000U : If invalid frame is passed as input
 * 		0x00900000U : If invalid qword is passed as input
 * 		0x00D00000U : If input bit is masked
 * 		0x00C00000U : If injection is failed
 *****************************************************************************/
XStatus XSem_CfrApiErrNjct (XSemCfrErrInjData *ErrData)
{
	XStatus Status = XST_FAILURE;
	XSemIpiResp IpiResp={0};

	Status = XSem_CmdCfrNjctErr(&IpiInst, ErrData, &IpiResp);

	if ((XST_SUCCESS == Status) && \
			(CMD_ACK_CFR_NJCT_ERR == IpiResp.RespMsg1)) {
		if (XST_SUCCESS != IpiResp.RespMsg2) {
			xil_printf("[%s] Error: Injection Failed with 0x%x\n", \
					__func__, IpiResp.RespMsg2);
			Status = (XStatus) IpiResp.RespMsg2;
		} else {
			xil_printf("[%s] Success: Inject\n\r", __func__);
		}
	} else {
		xil_printf("[%s] Error: Njct Status 0x%x Ack 0x%x Ret 0x%x\n", \
				__func__, Status, IpiResp.RespMsg1, \
				IpiResp.RespMsg2);
	}

	return Status;
}

/******************************************************************************
 * @brief	This function is used to inject 1-bit correctable error
 *
 * @return	Status : Success or Failure
 *****************************************************************************/
XStatus Xsem_CfrApiInjctCorErr()
{
	/* To inject correctable error, inject error in single bit position in
	 * any of the row/frame/qword. The argument details are
	 * ErrData[0]: Frame Address : 0, Quadword: 3, Bit position: 4, Row: 0
	 * Valid ranges: Row 0-3, Qword 0-24, Bit position 0-127,
	 * For frame address, refer to the LAST_FRAME_TOP (CFRAME_REG) and
	 * LAST_FRAME_BOT (CFRAME_REG) registers.
	 * In Qword 12, 0-23 & 48-71 are syndrome bits. All other bits are data bits.
	 * To inject single bit error in syndrome, select Qword 12 in any of the
	 * frame with bit position in range of 0-23 or 48-71
	 */
	XSemCfrErrInjData ErrData = {0, 3, 4, 0};
	XStatus Status = 0U;

	Status = XSem_CfrApiErrNjct(&ErrData);
	if(Status == XST_FAILURE){
		xil_printf("Error injection failed due to IPI failure\n\r");
	}

	return Status;
}

/******************************************************************************
 * @brief	This function is used to inject Uncorrectable error
 *
 * @return	Status : Success or Failure
 *****************************************************************************/
XStatus Xsem_CfrApiInjctUnCorErr()
{
	/* To inject uncorrectable error, the injection has to be done in alternate
	 * bit positions in the same qword. The argument details are
	 * ErrData[0]: Frame Address : 0, Quadword: 0, Bit position: 1, Row: 0
	 * ErrData[1]: Frame Address : 0, Quadword: 0, Bit position: 3, Row: 0
	 */
	XSemCfrErrInjData ErrData[2] = {{0, 0, 1, 0},
					{0, 0, 3, 0},
					};
	XStatus Status = 0U;
	u32 Index = 0U;

	for(Index = 0U; Index < 2U; Index++){
		Status = XSem_CfrApiErrNjct(&ErrData[Index]);
		if(Status == XST_FAILURE){
			xil_printf("Error injection failed due to" \
				" IPI failure\n\r");
			break;
		}
	}

	return Status;
}

/******************************************************************************
 * @brief	This function is used to inject CRC error. This error is treated as
 * uncorrectable error.
 *
 * @return	Status : Success or Failure
 *****************************************************************************/
XStatus Xsem_CfrApiInjctCrcErr()
{
	/* To inject CRC error, the injection has to be done in alternate bit
     * positions of the same qword in 3 or more than 3 positions.
	 * The argument details are
	 * ErrData[0]: Frame Address : 0, Quadword: 0, Bit position: 1, Row: 2
	 * ErrData[1]: Frame Address : 0, Quadword: 0, Bit position: 3, Row: 2
	 * ErrData[2]: Frame Address : 0, Quadword: 0, Bit position: 5, Row: 2
	 * ErrData[3]: Frame Address : 0, Quadword: 0, Bit position: 7, Row: 2
	 */
	XSemCfrErrInjData ErrData[4] = {{0, 0, 1, 2},
					{0, 0, 3, 2},
					{0, 0, 5, 2},
					{0, 0, 7, 2},
					};
	XStatus Status = 0U;
	u32 Index = 0U;

	for(Index = 0U; Index < 4U; Index++){
		Status = XSem_CfrApiErrNjct(&ErrData[Index]);
		if(Status == XST_FAILURE){
			xil_printf("Error injection failed due to" \
				" IPI failure\n\r");
			break;
		}
	}

	return Status;
}

/******************************************************************************
 * @brief	This function checks if correctable error counter is incremented
 * 	        after error injection is successful
 *
 * @param[in]	IntialCorErrCnt : Error count before error injection
 * @param[out]	ExpectedCount : Expected error count after injection
 *
 * @return	Status : Success or Failure
 *
 *****************************************************************************/
XStatus ApiChkErrorCount(u32 IntialCorErrCnt, u32 ExpectedCount)
{
	XStatus Status = XST_SUCCESS;
	XSemCfrStatus CfrStatusInfo = {0};
	u32 TotalCorErrCnt = 0U;

	(void)XSem_CmdCfrGetStatus(&CfrStatusInfo);
	TotalCorErrCnt = CfrStatusInfo.ErrCorCnt - IntialCorErrCnt;
	if(TotalCorErrCnt != ExpectedCount){
		Status = XST_FAILURE;
	}

	return Status;
}

/******************************************************************************
 * @brief	Verifies and Prints error report
 *
 * @param[in]	IntialCorErrCnt : Error count before error injection
 *
 *****************************************************************************/
void PrintErrReport(u32 IntialCorErrCnt)
{
	XStatus Status = 0U;
	XSemCfrStatus CfrStatusInfo = {0};
	u32 TempA = 0U;
	u32 TempB = 0U;
	u32 CorErrCount = 0U;
	u32 ErrIndex = 0U;
	u32 ErrBitLoc = 0U;
	u32 ErrQwordLoc = 0U;
	u32 ErrFarLoc = 0U;
	u32 ErrRowLoc = 0U;

	xil_printf("-----------------------------------------------------\n\r");
	xil_printf("-----------------Print Report------------------------\n\r");
	xil_printf("-----------------------------------------------------\n\r");
	/*Check error is detected and corrected*/
	(void)XSem_CmdCfrGetStatus(&CfrStatusInfo);
	TempA = (CRAM_STATUS_COR_ERR_MASK |
			CRAM_STATUS_CRC_ERR_MASK |
			CRAM_STATUS_UNCOR_ERR_MASK);
	Status = CfrStatusInfo.Status & TempA;

	if (Status == CRAM_STATUS_UNCOR_ERR_MASK) {

		xil_printf("[SUCCESS] UnCorrectable error detected \n\r");

		/*Check Error count increased as expected or not*/
		Status = ApiChkErrorCount(IntialCorErrCnt, 0U);
		if (Status == XST_SUCCESS){
			xil_printf("[SUCCESS] No increase in Error Count\n\r");
		} else {
			xil_printf("[FAILURE] Unexpected increase in" \
				" Error Count\n\r");
		}

		/*Check Event notification*/
		if(EventCnt_UnCorEcc == 1U && EventCnt_Crc == 0U &&
			EventCnt_CorEcc == 0U && EventCnt_IntErr == 0U){
			xil_printf("[SUCCESS] Received Uncorrectable error" \
				" event notification\n\r");
			EventCnt_UnCorEcc = 0U;
		} else {
			xil_printf("[FAILURE] No Uncorrectable error event" \
				" notification received\n\r");
		}

	} else if (Status == CRAM_STATUS_CRC_ERR_MASK) {

		xil_printf("[SUCCESS] CRC error detected \n\r");

		/*Check Error count increased as expected or not*/
		Status = ApiChkErrorCount(IntialCorErrCnt, 0U);
		if (Status == XST_SUCCESS){
			xil_printf("[SUCCESS] No increase in Error Count\n\r");
		} else {
			xil_printf("[FAILURE] Unexpected increase in" \
				" Error Count\n\r");
		}

		/*Check Event notification*/
		if(EventCnt_UnCorEcc == 0U && EventCnt_Crc == 1U &&
			EventCnt_CorEcc == 0U && EventCnt_IntErr == 0U){
			xil_printf("[SUCCESS] Received CRC error" \
				" event notification\n\r");
			EventCnt_Crc = 0U;
		} else {
			xil_printf("[FAILURE] No CRC error event" \
				" notification received\n\r");
		}

	} else if(Status == CRAM_STATUS_COR_ERR_MASK) {

		xil_printf("[SUCCESS] Correctable error detected \n\r");

		Status = CfrStatusInfo.Status & CRAM_STATUS_ECC_COR_DONE_MASK;
		/*To print and check Corrected Error count*/
		if (Status == CRAM_STATUS_ECC_COR_DONE_MASK) {

			xil_printf("[SUCCESS] Correction Done \n\r");

			CorErrCount = CfrStatusInfo.ErrCorCnt;
			xil_printf("Total Corrected Error count = %d\n\r", \
					CorErrCount);

			/*Check Error count increased as expected or not*/
			Status = ApiChkErrorCount(IntialCorErrCnt, 1U);
			if (Status == XST_SUCCESS){
				xil_printf("[SUCCESS] Error Count increased" \
					" by 1 as expected\n\r");
			} else {
				xil_printf("[FAILURE] XilSEM failed to" \
					" increase error count\n\r");
			}
		}

		/*Check Event notification*/
		if(EventCnt_UnCorEcc == 0U && EventCnt_Crc == 0U &&
			EventCnt_CorEcc == 1U && EventCnt_IntErr == 0U){
			xil_printf("[SUCCESS] Received Correctable error" \
				" event notification\n\r");
			EventCnt_CorEcc = 0U;
		} else {
			xil_printf("[FAILURE] No Correctable error event" \
				" notification received\n\r");
		}

		/*To print error location details*/
		ErrIndex = (CorErrCount % 7U);
		if(ErrIndex != 0U){
			--ErrIndex;
		} else{
			ErrIndex = 6U;
		}

		TempA = CfrStatusInfo.ErrAddrL[ErrIndex];
		TempB = CfrStatusInfo.ErrAddrH[ErrIndex];

		/* bit is at 22:16 bits of SEM_CRAMERR_ADDRL0 reg */
		ErrBitLoc 	= (TempA & 0x7F0000) >> 16;
		/* Qword is at 27:23 bits of SEM_CRAMERR_ADDRL0 reg */
		ErrQwordLoc = (TempA & 0xF800000) >> 23;
		/* Frame  is at 19:0 bits of SEM_CRAMERR_ADDRH0 reg */
		ErrFarLoc 	= (TempB & 0x7FFFFF);
		/* Frame  is at 26:23 bits of SEM_CRAMERR_ADDRH0 reg */
		ErrRowLoc 	= (TempB & 0x7800000) >> 23;

		xil_printf("Error is located in Row = %d, FAR = 0x%06x,"
			" Qword = %d, Bit = %d\n\r", ErrRowLoc, ErrFarLoc, \
			ErrQwordLoc, ErrBitLoc);

	} else {
		xil_printf("[FAILURE] XilSEM failed to detect error," \
			" Status = 0x%08x\n\r", CfrStatusInfo.Status);
	}
}
#endif /* End of XILSEM_ERRINJ_ENABLE */

/******************************************************************************
 * @brief	This function is used to Register XilSEM event notifications
 *
 * @param[in]	Enable : Enable event notification in XilSEM
 *
 * @return	Status : Success or Failure
 *
 *****************************************************************************/
int Xsem_CfrEventRegisterNotifier(u32 Enable)
{
	int Status;

	if (Enable) {
		Notifier.Flag = 1U;
	} else {
		Notifier.Flag = 0U;
	}
	/* In this example all CRAM events are enabled
	 * If you want to enable a particular event set the Event member in
	 * Notifier structure with corresponding event.
	 */
	Status = XSem_RegisterEvent(&IpiInst, &Notifier);

	return Status;
}

/******************************************************************************
 * @brief	IpiCallback to receive event messages
 *
 * @param[in]	InstancePtr : Pointer to IPI driver instance
 *
 *****************************************************************************/
void XSem_IpiCallback(XIpiPsu *const InstancePtr)
{
	int Status;
	u32 Payload[PAYLOAD_ARG_CNT] = {0};

	Status = XIpiPsu_ReadMessage(&IpiInst, IPI_PMC_MASK, Payload, \
			PAYLOAD_ARG_CNT, XIPIPSU_BUF_TYPE_MSG);
	if (Status != XST_SUCCESS) {
		xil_printf("ERROR #%d while reading IPI buffer\n", Status);
		return;
	}

	if ((XSEM_EVENT_ERROR == Payload[0]) && \
			(XSEM_NOTIFY_CRAM == Payload[1])) {
		if (XSEM_EVENT_CRAM_UNCOR_ECC_ERR == Payload[2]) {
			xil_printf("Received CRAM Uncorrecatble error event \n");
			EventCnt_UnCorEcc = 1U;
		} else if (XSEM_EVENT_CRAM_CRC_ERR == Payload[2]) {
			xil_printf("Received CRAM CRC error event \n");
			EventCnt_Crc = 1U;
		} else if (XSEM_EVENT_CRAM_INT_ERR == Payload[2]) {
			xil_printf("[ALERT] Received internal error event" \
				" notification from XilSEM\n\r");
			EventCnt_IntErr = 1U;
		} else if (XSEM_EVENT_CRAM_COR_ECC_ERR == Payload[2]) {
			xil_printf("Received CRAM Correctable error event \n");
			EventCnt_CorEcc = 1U;
		} else {
			xil_printf("%s Someother callback received: %d:%d:%d\n",
					__func__, Payload[0], \
					Payload[1], Payload[2]);
		}
	} else {
		xil_printf("%s Some other callback received: %d\n", \
				__func__, Payload[0]);
	}
}
/******************************************************************************/
/**
*
* This function connects the IPI interrupts to the interrupt controller and to
* the processor.
*
* @param	Intc instance pointer.
*
* @return	None.
*
****************************************************************************/
int SetUpInterruptSystem(XIntc *XIntcInstancePtr)
{
	int Status = XST_FAILURE;


	/**
	 * Connect IPI interrupt handler that will be called when an interrupt
	 * for the IPI occurs, the IPI interrupt handler performs the
	 * specific interrupt processing for IPI.
	 */
	Status = XIntc_Connect(XIntcInstancePtr, INTC_DEVICE_IPI_INT_ID,
				   (XInterruptHandler)XSem_IpiCallback,
				   (void *)0);
	if (Status != XST_SUCCESS) {
		goto END;
	}

	/**
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts.
	 */
	Status = XIntc_Start(XIntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		goto END;
	}

	/* Enable the interrupt for the device. */
	XIntc_Enable(XIntcInstancePtr, INTC_DEVICE_IPI_INT_ID);

	/* Initialize the exception table. */
	Xil_ExceptionInit();

	/* Register the interrupt controller handler with the exception table. */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				XIntcInstancePtr);

	/* Enable exceptions. */
	Xil_ExceptionEnable();

END:
	return Status;
}

/*****************************************************************************/
/**
*
* This function initializes Interrupt Controller and IPI drivers.
*
* @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
*
****************************************************************************/
int IntcAndIpiInit(void)
{
	int Status = XST_FAILURE;
	XIpiPsu_Config *CfgPtr;

	xil_printf("Initializing IPI start\r\n");

	/* Look Up the config data */
	CfgPtr = XIpiPsu_LookupConfig(IPI_CHANNEL_ID);
	xil_printf("IPI look up config done\r\n");

	/* Init with the Cfg Data */
	XIpiPsu_CfgInitialize(&IpiInst, CfgPtr, CfgPtr->BaseAddress);
	xil_printf("IPI config init done\r\n");

	/**
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		goto END;
	}
	xil_printf("Intc init done\r\n");

	/**
	 * Perform a self-test to ensure that the hardware was built
	 * correctly.
	 */
	Status = XIntc_SelfTest(&InterruptController);
	if (Status != XST_SUCCESS) {
		goto END;
	}
	xil_printf("Intc self test done\r\n");

	/* Setup the Interrupt System. */
	Status = SetUpInterruptSystem(&InterruptController);
	if (Status != XST_SUCCESS) {
		goto END;
	}
	xil_printf("setup interrupt done\r\n");

	/* Enable reception of IPIs from PMC */
	XIpiPsu_InterruptEnable(&IpiInst, IPI_PMC_MASK);
	xil_printf("IPI intr enabled\r\n");

	/* Clear Any existing Interrupts */
	XIpiPsu_ClearInterruptStatus(&IpiInst, IPI_PMC_MASK);
	xil_printf("IPI intr clear done\r\n");

END:
	return Status;
}

/***************************************************************************
 * @brief	Main showing example with usage of API's to StartScan, StopScan,
 *		& Error Inject
 **************************************************************************/
int main(void)
{
	XStatus Status = XST_SUCCESS;
	XSemCfrStatus CfrStatusInfo = {0};
	u32 IsInitDone = 0U;

#ifdef XILSEM_ERRINJ_ENABLE
	u32 IsErrorInjected = 0U;
	u32 IntialCorErrCnt = 0U;
#endif /* End of XILSEM_ERRINJ_ENABLE */

	XSemIpiResp IpiResp={0};
	u32 CframeAddr = 0xFU;
	u32 RowLoc = 0U;
	u32 GoldenCrc = 0U;
	u32 TotalFrames[7];
	u32 Id;
	u32 Index =0U;
	u32 FailCnt = 0;
	/**
	 * Initialize IPI Driver
	 * This initialization is required to get XilSEM event notifications
	 * from PLM
	 */
	Status = IntcAndIpiInit();
	if (Status != XST_SUCCESS) {
		xil_printf("IPI and INTC init failed: 0x%x\r\n", Status);
		xil_printf("Register notify example failed\r\n");
		FailCnt++;
		goto END;
	}

	/**
	 * Initialize CRAM scan if not initialized
	 * This is applicable when CRAM Scan is set for deferred start up.
	 */
	(void)XSem_CmdCfrGetStatus(&CfrStatusInfo);
	xil_printf("CRAM Scan Status:%x\n",CfrStatusInfo.Status);
	for(Index = 0U; Index < MAX_CRAMERR_REGISTER_CNT; Index++) {
		xil_printf("Last Corrected Location_%x High Addr: %x Low Addr: %x\n", \
			Index,CfrStatusInfo.ErrAddrH,CfrStatusInfo.ErrAddrL);
	}
	xil_printf("Total CE count: %x\n" ,CfrStatusInfo.ErrCorCnt);
	IsInitDone = CfrStatusInfo.Status & CRAM_STATUS_INIT_DONE_MASK;
	if (IsInitDone != CRAM_STATUS_INIT_DONE_MASK) {
		Status = XSem_CfrApiInitCram();
		if (XST_SUCCESS != Status) {
			xil_printf("CRAM Initialization failed\n\r");
			FailCnt++;
			goto END;
		} else {
			xil_printf("CRAM Initialization Done\n\r");
		}
	} else {
		xil_printf("CRAM scan is configured for immediate start\n\r");
	}

#ifdef XILSEM_ERRINJ_ENABLE
	/* Capture Initial error count */
	IntialCorErrCnt = CfrStatusInfo.ErrCorCnt;
#endif /* End of XILSEM_ERRINJ_ENABLE */

	/**
	 * Enable event Notifications to receive notifications from PLM
	 * upon detection of any error in CFRAME
	 */
	Xsem_CfrEventRegisterNotifier(1U);

	/* Read Frame ECC values of a particular frame over IPI */
	Status = XSem_CmdCfrReadFrameEcc(&IpiInst, \
				CframeAddr, RowLoc, &IpiResp);
	if ((XST_SUCCESS == Status) && \
			(CMD_ACK_SEM_READ_FRAME_ECC == IpiResp.RespMsg1) && \
			(XST_SUCCESS == IpiResp.RespMsg4)) {
		xil_printf("[XSem_CmdCfrReadFrameEcc] Success Reading Cfr Frame "
					"ECC Values of Row = 0x%02x  Frame = 0x%08x  Over IPI\n\r",\
				RowLoc, CframeAddr);
		xil_printf("Received Segment 0 ECC "
				"Value = 0x%08x\n\r", IpiResp.RespMsg2);
		xil_printf("Received Segment 1 ECC "
				"Value = 0x%08x\n\r", IpiResp.RespMsg3);
	} else {
		xil_printf("[%s] Error: Reading Cfr frame ECC, "
				"Status 0x%x Ack 0x%x Ret 0x%x\n", \
				__func__, Status, IpiResp.RespMsg1, \
						IpiResp.RespMsg4);
		FailCnt++;
	}

	/* Stop Scan */
	Status = XSem_CfrApiStopScan();
	if (XST_SUCCESS != Status) {
		xil_printf("Stop Scan Failed\n\r");
		FailCnt++;
		goto END;
	}

	/* Read Total number of frames in Row 0 */
	XSem_CmdCfrGetTotalFrames(RowLoc, &TotalFrames[RowLoc]);
	for(Id = 0U; Id < CFRAME_MAX_TYPE; Id++) {
		xil_printf("Type: %x Total Frames : %d \n", Id, TotalFrames[Id]);
	}

	/* Read Golden ECC for Row 0 */
	GoldenCrc = XSem_CmdCfrGetCrc(RowLoc);
	xil_printf("Golden CRC for ROW_0 : %x \n", GoldenCrc);

	/* Start Scan */
	Status = XSem_CfrApiStartScan();
	if (XST_SUCCESS != Status) {
		xil_printf("start Scan Failed\n\r");
		FailCnt++;
		goto END;
	}

	/* Read CRAM and NPI configuration over IPI */
	Status = XSem_CmdGetConfig(&IpiInst, &IpiResp);
	if ((XST_SUCCESS == Status) && \
			(CMD_ACK_SEM_GET_CONFIG == IpiResp.RespMsg1) && \
			(XST_SUCCESS == IpiResp.RespMsg4)) {
		xil_printf("[XSem_CmdGetConfig] Success Reading CRAM"
				" and NPI configuration Over IPI\n\r");
		xil_printf("Received CRAM Configuration ="
				" 0x%08x\n\r", IpiResp.RespMsg2);
		xil_printf("Received NPI Configuration ="
				" 0x%08x\n\r", IpiResp.RespMsg3);
	} else {
		xil_printf("[%s] Error: Reading CRAM and NPI Configuration, "
				"Status 0x%x Ack 0x%x Ret 0x%x\n", \
				__func__, Status, IpiResp.RespMsg1, \
						IpiResp.RespMsg4);
		FailCnt++;
	}

#ifdef XILSEM_ERRINJ_ENABLE
	/* The following sequence demonstrates how to inject errors in CRAM
	 * 1. StopScan
	 * 2. InjectError
	 * 3. StartScan
	 * 4. Read Correctable error count in case of correctable errors and status
	 * 5. For Error injection feature check enable XILSEM_ERRINJ_ENABLE macro
	 */
	/* Stop Scan */
	Status = XSem_CfrApiStopScan();
	if (XST_SUCCESS != Status) {
		xil_printf("Stop Scan Failed\n\r");
		FailCnt++;
		goto END;
	}

	/**
	 * Error injection : This example demonstrates how to inject 1-bit
	 * Correctable error. If you want to inject CRC error, you need to
	 * replace the function call Xsem_CfrApiInjctCorErr
	 * with Xsem_CfrApiInjctCrcErr.
	 * If you want to inject uncorrectable error replace the function call
	 * with Xsem_CfrApiInjctUnCorErr.
	 */

	Status = Xsem_CfrApiInjctCorErr();
	if (Status == XST_SUCCESS){
		IsErrorInjected = 1U;
	}
	else
	{
		FailCnt++;
	}

	/**
	 * Start Scan to enable CRAM to detect the injected error.
	 * In case of CRC or uncorrectable error, XilSEM stops the scan
	 * and enters into idle state which means no further scan will be
	 * performed.
	 * In case of correctable error, XilSEM corrects the error
	 * if the correction is enabled in the configuration. Else, it will
	 * report the error and enters into idle state.
	 */
	Status = XSem_CfrApiStartScan();
	if (XST_SUCCESS != Status) {
		xil_printf("Start Scan Failed\n\r");
		FailCnt++;
		goto END;
	}

	if (IsErrorInjected != 1U)
		{
			FailCnt++;
			goto END;
		}

	/* Wait for the error to be detected */
	sleep(2);

	/* Check and print error report */
	PrintErrReport(IntialCorErrCnt);
#endif /* End of XILSEM_ERRINJ_ENABLE */

END:
	xil_printf("\n\r-------------- Test Report --------------\n\r");
	xil_printf("Failed Command Count : %d\n\r", FailCnt);
	if(FailCnt) {
		xil_printf("CRAM example Failed \n");
	}else{
		xil_printf("CRAM examples ran successfully \n");
	}
	xil_printf("-----------------------------------------\n\r");
	return 0;
}
