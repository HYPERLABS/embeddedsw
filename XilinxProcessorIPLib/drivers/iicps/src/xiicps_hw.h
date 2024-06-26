/******************************************************************************
* Copyright (C) 2010 - 2021 Xilinx, Inc.  All rights reserved.
* Copyright (C) 2022 - 2024 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xiicps_hw.h
* @addtogroup iicps_api IICPS APIs
* @{
*
* The xiicps_hw.h header file contains the hardware definition for an IIC device.
* It includes register definitions and interface functions to read/write
* the registers.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who 	Date     Changes
* ----- ------  -------- -----------------------------------------------
* 1.00a drg/jz  01/30/10 First release
* 1.04a kpc		11/07/13 Added function prototype.
* 3.0	sk		11/03/14 Modified the TimeOut Register value to 0xFF
*				01/31/15 Modified the code according to MISRAC 2012 Compliant.
* 3.10  sg 		06/24/19 Added timeout macro for polled transfer.
* </pre>
*
******************************************************************************/
#ifndef XIICPS_HW_H		/* prevent circular inclusions */
#define XIICPS_HW_H		/**< by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"

/************************** Constant Definitions *****************************/

/** @name Register Map
 *
 * Register offsets for the IIC.
 * @{
 */
#define XIICPS_CR_OFFSET			0x00U  /**< 32-bit Control */
#define XIICPS_SR_OFFSET			0x04U  /**< Status */
#define XIICPS_ADDR_OFFSET			0x08U  /**< IIC Address */
#define XIICPS_DATA_OFFSET			0x0CU  /**< IIC FIFO Data */
#define XIICPS_ISR_OFFSET			0x10U  /**< Interrupt Status */
#define XIICPS_TRANS_SIZE_OFFSET	0x14U  /**< Transfer Size */
#define XIICPS_SLV_PAUSE_OFFSET		0x18U  /**< Slave monitor pause */
#define XIICPS_TIME_OUT_OFFSET		0x1CU  /**< Time Out */
#define XIICPS_IMR_OFFSET			0x20U  /**< Interrupt Enabled Mask */
#define XIICPS_IER_OFFSET			0x24U  /**< Interrupt Enable */
#define XIICPS_IDR_OFFSET			0x28U  /**< Interrupt Disable */
/** @} */

/** @cond INTERNAL */
/** @name Control Register
 *
 * This register contains various control bits that
 * affects the operation of the IIC controller. Read/Write.
 * @{
 */
#define XIICPS_CR_DIV_A_MASK	0x0000C000U /**< Clock Divisor A */
#define XIICPS_CR_DIV_A_SHIFT			14U /**< Clock Divisor A shift */
#define XIICPS_DIV_A_MAX				 4U /**< Maximum value of Divisor A */
#define XIICPS_CR_DIV_B_MASK	0x00003F00U /**< Clock Divisor B */
#define XIICPS_CR_DIV_B_SHIFT			 8U /**< Clock Divisor B shift */
#define XIICPS_CR_CLR_FIFO_MASK	0x00000040U /**< Clear FIFO, auto clears*/
#define XIICPS_CR_SLVMON_MASK	0x00000020U /**< Slave monitor mode */
#define XIICPS_CR_HOLD_MASK		0x00000010U /**<  Hold bus 1=Hold scl,
												0=terminate transfer */
#define XIICPS_CR_ACKEN_MASK	0x00000008U /**< Enable TX of ACK when
												Master receiver*/
#define XIICPS_CR_NEA_MASK		0x00000004U /**< Addressing Mode 1=7 bit,
												0=10 bit */
#define XIICPS_CR_MS_MASK		0x00000002U /**< Master mode bit 1=Master,
												0=Slave */
#define XIICPS_CR_RD_WR_MASK	0x00000001U /**< Read or Write Master
												transfer  0=Transmitter,
												1=Receiver*/
#define XIICPS_CR_RESET_VALUE			 0U /**< Reset value of the Control
												register */
/** @} */

/** @name IIC Status Register
 *
 * This register is used to indicate status of the IIC controller. Read only
 * @{
 */
#define XIICPS_SR_BA_MASK		0x00000100U  /**< Bus Active Mask */
#define XIICPS_SR_RXOVF_MASK	0x00000080U  /**< Receiver Overflow Mask */
#define XIICPS_SR_TXDV_MASK		0x00000040U  /**< Transmit Data Valid Mask */
#define XIICPS_SR_RXDV_MASK		0x00000020U  /**< Receiver Data Valid Mask */
#define XIICPS_SR_RXRW_MASK		0x00000008U  /**< Receive read/write Mask */
/** @} */

/** @name IIC Address Register
 *
 * Normal addressing mode uses add[6:0]. Extended addressing mode uses add[9:0].
 * A write access to this register always initiates a transfer if the IIC is in
 * master mode. Read/Write
 * @{
 */
#define XIICPS_ADDR_MASK	0x000003FF  /**< IIC Address Mask */
/** @} */

/** @name IIC Data Register
 *
 * When written to, the data register sets data to transmit. When read from, the
 * data register reads the last received byte of data. Read/Write
 * @{
 */
#define XIICPS_DATA_MASK	0x000000FF  /**< IIC Data Mask */
/** @} */
/** @endcond */

/** @name IIC Interrupt Registers
 *
 * <b>IIC Interrupt Status Register</b>
 *
 * This register holds the interrupt status flags for the IIC controller. Some
 * of the flags are level triggered
 * - i.e. are set as long as the interrupt condition exists.  Other flags are
 *   edge triggered, which means they are set one the interrupt condition occurs
 *   then remain set until they are cleared by software.
 *   The interrupts are cleared by writing a one to the interrupt bit position
 *   in the Interrupt Status Register. Read/Write.
 *
 * <b>IIC Interrupt Enable Register</b>
 *
 * This register is used to enable interrupt sources for the IIC controller.
 * Writing a '1' to a bit in this register clears the corresponding bit in the
 * IIC Interrupt Mask register.  Write only.
 *
 * <b>IIC Interrupt Disable Register </b>
 *
 * This register is used to disable interrupt sources for the IIC controller.
 * Writing a '1' to a bit in this register sets the corresponding bit in the
 * IIC Interrupt Mask register. Write only.
 *
 * <b>IIC Interrupt Mask Register</b>
 *
 * This register shows the enabled/disabled status of each IIC controller
 * interrupt source. A bit set to 1 will ignore the corresponding interrupt in
 * the status register. A bit set to 0 means the interrupt is enabled.
 * All mask bits are set and all interrupts are disabled after reset. Read only.
 *
 * All four registers have the same bit definitions. They are only defined once
 * for each of the Interrupt Enable Register, Interrupt Disable Register,
 * Interrupt Mask Register, and Interrupt Status Register
 * @{
 */

#define XIICPS_IXR_ARB_LOST_MASK  0x00000200U	 /**< Arbitration Lost Interrupt
													mask */
#define XIICPS_IXR_RX_UNF_MASK    0x00000080U	 /**< FIFO Receive Underflow
													Interrupt mask */
#define XIICPS_IXR_TX_OVR_MASK    0x00000040U	 /**< Transmit Overflow
													Interrupt mask */
#define XIICPS_IXR_RX_OVR_MASK    0x00000020U	 /**< Receive Overflow Interrupt
													mask */
#define XIICPS_IXR_SLV_RDY_MASK   0x00000010U	 /**< Monitored Slave Ready
													Interrupt mask */
#define XIICPS_IXR_TO_MASK        0x00000008U	 /**< Transfer Time Out
													Interrupt mask */
#define XIICPS_IXR_NACK_MASK      0x00000004U	 /**< NACK Interrupt mask */
#define XIICPS_IXR_DATA_MASK      0x00000002U	 /**< Data Interrupt mask */
#define XIICPS_IXR_COMP_MASK      0x00000001U	 /**< Transfer Complete
													Interrupt mask */
#define XIICPS_IXR_DEFAULT_MASK   0x000002FFU	 /**< Default ISR Mask */
#define XIICPS_IXR_ALL_INTR_MASK  0x000002FFU	 /**< All ISR Mask */
/** @} */

/** @cond INTERNAL */
/** @name IIC Transfer Size Register
*
* The register's meaning varies according to the operating mode as follows:
*   - Master transmitter mode: number of data bytes still not transmitted minus
*     one
*   - Master receiver mode: number of data bytes that are still expected to be
*     received
*   - Slave transmitter mode: number of bytes remaining in the FIFO after the
*     master terminates the transfer
*   - Slave receiver mode: number of valid data bytes in the FIFO
*
* This register is cleared if CLR_FIFO bit in the control register is set.
* Read/Write
* @{
*/
#define XIICPS_TRANS_SIZE_MASK  0x0000003F /**< IIC Transfer Size Mask */
#define XIICPS_FIFO_DEPTH          16	  /**< Number of bytes in the FIFO */
#define XIICPS_DATA_INTR_DEPTH     14    /**< Number of bytes at DATA intr */
/** @} */


/** @name IIC Slave Monitor Pause Register
*
* This register is associated with the slave monitor mode of the I2C interface.
* It is meaningful only when the module is in master mode and bit SLVMON in the
* control register is set.
*
* This register defines the pause interval between consecutive attempts to
* address the slave once a write to an I2C address register is done by the
* host. It represents the number of sclk cycles minus one between two attempts.
*
* The reset value of the register is 0, which results in the master repeatedly
* trying to access the slave immediately after unsuccessful attempt.
* Read/Write
* @{
*/
#define XIICPS_SLV_PAUSE_MASK    0x0000000F  /**< Slave monitor pause mask */
/** @} */


/** @name IIC Time Out Register
*
* The value of time out register represents the time out interval in number of
* sclk cycles minus one.
*
* When the accessed slave holds the sclk line low for longer than the time out
* period, thus prohibiting the I2C interface in master mode to complete the
* current transfer, an interrupt is generated and TO interrupt flag is set.
*
* The reset value of the register is 0x1f.
* Read/Write
* @{
 */
#define XIICPS_TIME_OUT_MASK    0x000000FFU    /**< IIC Time Out mask */
#define XIICPS_TO_RESET_VALUE   0x000000FFU    /**< IIC Time Out reset value */
/** @} */

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define XIicPs_In32 Xil_In32		/**< XIicPs read */
#define XIicPs_Out32 Xil_Out32		/**< XIicPs write */
#define XIICPS_POLL_DEFAULT_TIMEOUT_VAL		1000U /**< Timeout in us */
/** @endcond */

/****************************************************************************/
/**
* Reads an IIC register.
*
* @param	BaseAddress Contains the base address of the device.
* @param	RegOffset Contains the offset from the 1st register of the
*		device to select the specific register.
*
* @return	The value read from the register.
*
* @note		C-Style signature:
*		u32 XIicPs_ReadReg(u32 BaseAddress. int RegOffset)
*
******************************************************************************/
#define XIicPs_ReadReg(BaseAddress, RegOffset) \
	XIicPs_In32((BaseAddress) + (u32)(RegOffset))

/***************************************************************************/
/**
* Writes an IIC register.
*
* @param	BaseAddress Contains the base address of the device.
* @param	RegOffset Contains the offset from the 1st register of the
*		device to select the specific register.
* @param	RegisterValue Value to be written to the register.
*
* @return	None.
*
* @note	C-Style signature:
*	void XIicPs_WriteReg(u32 BaseAddress, int RegOffset, u32 RegisterValue)
*
******************************************************************************/
#define XIicPs_WriteReg(BaseAddress, RegOffset, RegisterValue) \
	XIicPs_Out32((BaseAddress) + (u32)(RegOffset), (u32)(RegisterValue))

/***************************************************************************/
/**
* Reads the interrupt enable register.
*
* @param	BaseAddress Contains the base address of the device.
*
* @return	Current bit mask that represents currently enabled interrupts.
*
* @note		C-Style signature:
*		u32 XIicPs_ReadIER(u32 BaseAddress)
*
******************************************************************************/
#define XIicPs_ReadIER(BaseAddress) \
	XIicPs_ReadReg((BaseAddress),  XIICPS_IER_OFFSET)

/***************************************************************************/
/**
* Writes to the interrupt enable register.
*
* @param	BaseAddress Contains the base address of the device.
*
* @param	IntrMask Interrupts to be enabled.
*
* @return	None.
*
* @note	C-Style signature:
*	void XIicPs_EnabledInterrupts(u32 BaseAddress, u32 IntrMask)
*
******************************************************************************/
#define XIicPs_EnableInterrupts(BaseAddress, IntrMask) \
	XIicPs_WriteReg((BaseAddress), XIICPS_IER_OFFSET, (IntrMask))

/***************************************************************************/
/**
* Disables all interrupts.
*
* @param	BaseAddress Contains the base address of the device.
*
* @return	None.
*
* @note		C-Style signature:
*		void XIicPs_DisableAllInterrupts(u32 BaseAddress)
*
******************************************************************************/
#define XIicPs_DisableAllInterrupts(BaseAddress) \
	XIicPs_WriteReg((BaseAddress), XIICPS_IDR_OFFSET, \
		XIICPS_IXR_ALL_INTR_MASK)

/***************************************************************************/
/**
* Disables selected interrupts.
*
* @param	BaseAddress Contains the base address of the device.
*
* @param	IntrMask Interrupts to be disabled.
*
* @return	None.
*
* @note		C-Style signature:
*		void XIicPs_DisableInterrupts(u32 BaseAddress, u32 IntrMask)
*
******************************************************************************/
#define XIicPs_DisableInterrupts(BaseAddress, IntrMask) \
	XIicPs_WriteReg((BaseAddress), XIICPS_IDR_OFFSET, \
		(IntrMask))

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
/*
 * Perform reset operation to the I2c interface
 */
/** @cond INTERNAL */
void XIicPs_ResetHw(UINTPTR BaseAddress);
/** @endcond */
#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */
/** @} */
