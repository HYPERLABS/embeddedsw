/******************************************************************************
* Copyright (C) 2017 - 2022 Xilinx, Inc.  All rights reserved.
* Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
 ******************************************************************************/

/*
   DP159

   Written by Marco Groeneveld
   (c) 2014, 2015 by Xilinx Inc.

   History
   -------
   v1.0 - Initial release
   v1.1 - Updated DP159 setting to automatic redriver to retimer for HDMI 1.4
   data rates
 */

#include "dp159.h"
#if defined (__MICROBLAZE__)
#include "microblaze_sleep.h"
#else
#include "sleep.h"
#endif
#include "xiic.h"

#define DP159_VERBOSE	0
#define DP159_ZOMBIE 	0
#define DP159_ES		1

#define I2C_DP159_ZOMBIE_ADDR 	0x2C
#define I2C_DP159_ES_ADDR		0x5E

 /*****************************************************************************/
 /**
  *
  * I2C DP159 check
  * This routine checks if any data can be read from the DP159
  *
  * @param	dev is a flag for device address of DP I2C slave.
  *
  * @return
  *		- XST_SUCCESS when device is found successfully
  *		- XST_FAILURE otherwise.
  *
  * @note	None.
  *
 ******************************************************************************/
u8 i2c_dp159_chk(u8 dev) {
	u32 r;
	u8 buf[1];

	/* DP159 ES */
	if (dev == DP159_ES) {
#ifndef SDT
		r = XIic_Recv(XPAR_IIC_1_BASEADDR, I2C_DP159_ES_ADDR,
				(u8 *)&buf, 1, XIIC_STOP);
#else
		r = XIic_Recv(XPAR_XIIC_1_BASEADDR, I2C_DP159_ES_ADDR,
						(u8 *)&buf, 1, XIIC_STOP);
#endif
	}
	else
#ifndef SDT
		r = XIic_Recv(XPAR_IIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
				(u8 *)&buf, 1, XIIC_STOP);
#else
		r = XIic_Recv(XPAR_XIIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
					(u8 *)&buf, 1, XIIC_STOP);
#endif

	/* When a device is found, it returns one byte */
	if (r == 1)
		return XST_SUCCESS;
	else
		return XST_FAILURE;
}


 /*****************************************************************************/
 /**
  *
  * I2C DP159 write
  * This function writes a data to the DP159 I2C serial device
  *
  * @param	dev is a flag for device address of DP159 I2C slave.
  * @param	addr is address location of DP159 I2C device.
  * @param	dat is data be written to DP159 I2C device.
  *
  * @return
  *		- XST_SUCCESS when device is found successfully
  *		- XST_FAILURE otherwise.
  *
  * @note	None.
  *
 ******************************************************************************/
u32 i2c_dp159_write(u8 dev, u8 addr, u8 dat)
{
	u32 r;
	u8 buf[2];

	buf[0] = addr;
	buf[1] = dat;

	/* DP159 ES */
	if (dev == DP159_ES) {
#ifndef SDT
		r = XIic_Send(XPAR_IIC_1_BASEADDR, I2C_DP159_ES_ADDR,
				(u8 *)&buf, 2, XIIC_STOP);
#else
		r = XIic_Send(XPAR_XIIC_1_BASEADDR, I2C_DP159_ES_ADDR,
						(u8 *)&buf, 2, XIIC_STOP);
#endif
	}

	/* Zombie */
	else {
#ifndef SDT
		r = XIic_Send(XPAR_IIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
				(u8 *)&buf, 2, XIIC_STOP);
#else
		r = XIic_Send(XPAR_XIIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
					(u8 *)&buf, 2, XIIC_STOP);
#endif
	}

	if (r == 2)
		return XST_SUCCESS;
	else
		return XST_FAILURE;
}

 /*****************************************************************************/
 /**
  *
  * I2C DP159 read
  * This function reads a data from the DP159 I2C serial device
  *
  * @param	dev is a flag for device address of DP159 I2C slave.
  * @param	addr is address location of DP159 I2C device.
  *
  * @return
  *		- XST_SUCCESS when data is received successfully
  *		- XST_FAILURE otherwise.
  *
  * @note	None.
  *
 ******************************************************************************/
u8 i2c_dp159_read(u8 dev, u8 addr)
{
	u32 r;
	u8 buf[2];

	buf[0] = addr;

	/* DP159 ES */
	if (dev == DP159_ES) {
#ifndef SDT
		r = XIic_Send(XPAR_IIC_1_BASEADDR, I2C_DP159_ES_ADDR,
				(u8 *)&buf, 1, XII_REPEATED_START_OPTION);
		r = XIic_Recv(XPAR_IIC_1_BASEADDR, I2C_DP159_ES_ADDR,
				(u8 *)&buf, 1, XIIC_STOP);
#else
		r = XIic_Send(XPAR_XIIC_1_BASEADDR, I2C_DP159_ES_ADDR,
						(u8 *)&buf, 1, XII_REPEATED_START_OPTION);
		r = XIic_Recv(XPAR_XIIC_1_BASEADDR, I2C_DP159_ES_ADDR,
						(u8 *)&buf, 1, XIIC_STOP);
#endif
	}

	/* Zombie */
	else {
#ifndef SDT
		r = XIic_Send(XPAR_IIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
				(u8 *)&buf, 1, XII_REPEATED_START_OPTION);
		r = XIic_Recv(XPAR_IIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
				(u8 *)&buf, 1, XIIC_STOP);
#else
		r = XIic_Send(XPAR_XIIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
						(u8 *)&buf, 1, XII_REPEATED_START_OPTION);
		r = XIic_Recv(XPAR_XIIC_1_BASEADDR, I2C_DP159_ZOMBIE_ADDR,
						(u8 *)&buf, 1, XIIC_STOP);
#endif
	}

	if (r == 1)
		return buf[0];
	else
		return 0;
}

 /*****************************************************************************/
 /**
  *
  * I2C DP159 dump
  * This function dumps the DP159 ES I2c slave registers
  *
  * @param	None
  *
  * @return	None
  *
  * @note	None.
  *
 ******************************************************************************/
void i2c_dp159_dump(void)
{
	u32 r;
	u8 i;
	u8 buf[32];

	buf[0] = 0x0;
	xil_printf("DP159 register dump\n");
#ifndef SDT
	r = XIic_Send(XPAR_IIC_1_BASEADDR, I2C_DP159_ES_ADDR, (u8 *)&buf,
			1, XII_REPEATED_START_OPTION);

	r = XIic_Recv(XPAR_IIC_1_BASEADDR, I2C_DP159_ES_ADDR, (u8 *)&buf,
			32, XIIC_STOP);
#else
	r = XIic_Send(XPAR_XIIC_1_BASEADDR, I2C_DP159_ES_ADDR, (u8 *)&buf,
				1, XII_REPEATED_START_OPTION);

	r = XIic_Recv(XPAR_XIIC_1_BASEADDR, I2C_DP159_ES_ADDR, (u8 *)&buf,
				32, XIIC_STOP);
#endif
	for (i = 0; i<= 0x20; i++) {
		xil_printf("(%d) ADDR: %0x DATA: %0x\n", r, i, buf[i]);
	}
}

 /*****************************************************************************/
 /**
  *
  * I2C DP159
  * This function programs the DP159 I2c slave registers
  *
  * @param	VphyPtr is pointer to video phy controller
  * @param	QuadId	is a flag
  * @param	TxLineRate is a HDMI line rate.
  *
  * @return
  *		- XST_SUCCESS when I2C writes are successfully
  *		- XST_FAILURE otherwise.
  *
  * @note	None.
  *
 ******************************************************************************/
u32 i2c_dp159(XVphy *VphyPtr, u8 QuadId, u64 TxLineRate)
{
	u32 r;
	u8 mode;

	/* Select mode  */
	/* HDMI 2.0 */
	if ((TxLineRate / (1000000)) > 3400)
		mode = 2;

	/* HDMI 1.4 > 1.2 Gbps */
	else if ((TxLineRate / (1000000)) > 1200)
		mode = 1;

	/* HDMI 1.4 < 1.2 Gbps */
	else
		mode = 0;

	/* Check if the DP159 is a zombie device */
	r = i2c_dp159_chk(DP159_ZOMBIE);

	if (r == XST_SUCCESS) {
		if (DP159_VERBOSE)
			print("Program DP159 ZOMBIE... \n");
		switch(mode) {
			case 0 : /* HDMI 1.4 (250Mbps - 1.2Gbps) */
				/* Select page 1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xff, 0x01);

				/* PLL_FBDIV is 280 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x04, 0x80);
				r = i2c_dp159_write(DP159_ZOMBIE, 0x05, 0x02);

				/* PLL_PREDIV is 2 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x08, 0x02);

				/* CDR_CONFIG[4:0] */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x0e, 0x10);

				/* CP_CURRENT */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x01, 0x81);
#if defined (__MICROBLAZE__)
				MB_Sleep(10);
#else
				usleep(10000);
#endif

				/* Enable Bandgap */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x00, 0x02);
#if defined (__MICROBLAZE__)
				MB_Sleep(10);
#else
				usleep(10000);
#endif
				/* Enable PLL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x00, 0x03);

				/* Enable TX */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x10, 0x0f);

				/* HDMI_TWPST1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x14, 0x10);

				/* DP_TWPST1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x16, 0x10);

				/* DP_TWPST2 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x17, 0x00);

				/* Slew CTRL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x12, 0x28);

				/* FIR_UPD */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x13, 0x0f);
				r = i2c_dp159_write(DP159_ZOMBIE, 0x13, 0x00);

				/* TX_RATE */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x11, 0xC0);

				/* Enable receivers */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x30, 0x0f);

				/* PD_RXINT */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x32, 0x00);

				/* RX_RATE */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x31, 0xC0);

				/* Disable offset correction */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x34, 0x00);

				/* Change default of CDR_STL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x3c, 0x04);

				/* Change default of CDR_SO_TR */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x3D, 0x06);

				/* EQFTC */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x4D, 0x38);

				/* Enable Adaptive EQ */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x4c, 0x03);

				/* Select page 0 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xff, 0x00);

				/* Gate HPD_SNK */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x09, 0x01);

				/* Set GPIO */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xe0, 0x01);

				/* Un gate HPD_SNK */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x09, 0x00);
				return XST_SUCCESS;
				break;

			case 1 : /* HDMI 1.4 (1.2Gbps - 3Gbps) */
				/* Select page 1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xff, 0x01);

				/* PLL_FBDIV is 140 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x04, 0x40);
				r = i2c_dp159_write(DP159_ZOMBIE, 0x05, 0x01);

				/* PLL_PREDIV is 4 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x08, 0x04);

				/* CDR_CONFIG[4:0] */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x0e, 0x10);

				/* CP_CURRENT */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x01, 0x81);
#if defined (__MICROBLAZE__)
				MB_Sleep(10);
#else
				usleep(10000);
#endif
				/* Enable Bandgap */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x00, 0x02);
#if defined (__MICROBLAZE__)
				MB_Sleep(10);
#else
				usleep(10000);
#endif
				/* Enable PLL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x00, 0x03);

				/* Enable TX */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x10, 0x0f);

				/* HDMI_TWPST1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x14, 0x10);

				/* DP_TWPST1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x16, 0x10);

				/* DP_TWPST2 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x17, 0x00);

				/* Slew CTRL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x12, 0x28);

				/* FIR_UPD */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x13, 0x0f);
				r = i2c_dp159_write(DP159_ZOMBIE, 0x13, 0x00);

				/* TX_RATE */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x11, 0x70);

				/* Enable receivers */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x30, 0x0f);

				/* PD_RXINT */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x32, 0x00);

				/* RX_RATE */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x31, 0x40);

				/* Disable offset correction */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x34, 0x00);

				/* Change default of CDR_STL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x3c, 0x04);

				/* Change default of CDR_SO_TR */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x3D, 0x06);

				/* EQFTC */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x4D, 0x28);

				/* Enable Adaptive EQ */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x4c, 0x03);

				/* Select page 0 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xff, 0x00);

				/* Gate HPD_SNK */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x09, 0x01);

				/* Set GPIO */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xe0, 0x01);

				/* Un gate HPD_SNK */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x09, 0x00);
				return XST_SUCCESS;
				break;

			case 2 : /* HDMI 2.0 (3.4Gbps - 6 Gbps) */
				if (DP159_VERBOSE)
					xil_printf("Program zombie HDMI 2.0\n");

				/* Select page 1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xff, 0x01);

				/* PLL_FBDIV is 280 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x04, 0x80);
				r = i2c_dp159_write(DP159_ZOMBIE, 0x05, 0x02);

				/* PLL_PREDIV is 4 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x08, 0x04);

				/* CDR_CONFIG[4:0] */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x0e, 0x10);

				/* CP_CURRENT */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x01, 0x81);
#if defined (__MICROBLAZE__)
				MB_Sleep(10);
#else
				usleep(10000);
#endif
				/* Enable Bandgap */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x00, 0x02);
#if defined (__MICROBLAZE__)
				MB_Sleep(10);
#else
				usleep(10000);
#endif
				/* Enable PLL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x00, 0x03);

				/* Enable TX */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x10, 0x0f);

				/* HDMI_TWPST1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x14, 0x10);

				/* DP_TWPST1 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x16, 0x10);

				/* DP_TWPST2 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x17, 0x00);

				/* Slew CTRL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x12, 0x28);

				/* FIR_UPD */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x13, 0x0f);
				r = i2c_dp159_write(DP159_ZOMBIE, 0x13, 0x00);

				/* TX_RATE */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x11, 0x30);

				/* Enable receivers */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x30, 0x0f);

				/* PD_RXINT */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x32, 0x00);

				/* RX_RATE */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x31, 0x00);

				/* Disable offset correction */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x34, 0x00);

				/* Change default of CDR_STL */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x3c, 0x04);

				/* Change default of CDR_SO_TR */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x3D, 0x06);

				/* EQFTC */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x4D, 0x18);

				/* Enable Adaptive EQ */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x4c, 0x03);

				/* Select page 0 */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xff, 0x00);

				/* Gate HPD_SNK */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x09, 0x01);

				/* Set GPIO */
				r = i2c_dp159_write(DP159_ZOMBIE, 0xe0, 0x01);

				/* Un gate HPD_SNK */
				r = i2c_dp159_write(DP159_ZOMBIE, 0x09, 0x00);
				return XST_SUCCESS;
				break;
		}
	} else {
		/* Check if the DP159 is a ES device */
		r = i2c_dp159_chk(DP159_ES);

		if (r == XST_SUCCESS) {
			/*print("Program DP159 ES... \n"); */
			r = i2c_dp159_write(DP159_ES, 0x09, 0x06);

			/* HDMI 2.0 */
			if ((TxLineRate / (1000000)) > 3400) {
				if (DP159_VERBOSE)
					xil_printf("DP159 HDMI 2.0\n");
				/* Automatic retimer for HDMI 2.0 */
				r = i2c_dp159_write(DP159_ES, 0x0A, 0x36);
				r = i2c_dp159_write(DP159_ES, 0x0B, 0x1a);


				r = i2c_dp159_write(DP159_ES, 0x0C, 0xa1);
				r = i2c_dp159_write(DP159_ES, 0x0D, 0x00);
			}
			/* HDMI 1.4 */
			else {
				if (DP159_VERBOSE)
					xil_printf("DP159 HDMI 1.4\n");
				/*
				 * Automatic redriver to retimer crossover
				 * at 1.0 Gbps
				 */
				r = i2c_dp159_write(DP159_ES, 0x0A, 0x35);
				r = i2c_dp159_write(DP159_ES, 0x0B, 0x01);
				/* Set VSWING data decrease by 24% */
				r = i2c_dp159_write(DP159_ES, 0x0C, 0xA0);
				r = i2c_dp159_write(DP159_ES, 0x0D, 0x00);
			}
			return XST_SUCCESS;
		} else {
			print("No DP159 device found!\n");
			return XST_FAILURE;
		}
	}
	return XST_FAILURE;
}
