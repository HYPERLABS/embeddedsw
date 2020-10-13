/******************************************************************************
* Copyright (c) 2017 - 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file		xilsecure_rsa_generic_example.c
* @addtogroup xsecure_rsa_gen_apis XilSecure RSA GENERIC APIs
* @{
* This example illustrates RSA APIs usage, by taking some hash with PKCS
* padding.
*- Firstly data will be decrypted with Private key and generates RSA
* signature also verifies with expected signature.
* - Then signature will be encrypted with public key and generates the actual
* data also verifies with actual data.
*
* MODIFICATION HISTORY:
* <pre>
* Ver   Who    Date     Changes
* ----- ------ -------- -------------------------------------------------
* 2.2   vns    08/18/17 First release.
* 4.3   har    10/12/20 Addressed security review comments
*
* </pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xsecure_rsa.h"
#include "xsecure_sha.h"

/************************** Constant Definitions *****************************/
#define XSECURE_RSA_SIZE	512	/**< 512 bytes for 4096 bit data */
/**************************** Type Definitions *******************************/
/* Exponent of private key */
static u8 PrivateExp[XSECURE_RSA_SIZE] = {
	0x9e,0x65,0x5c,0xfb,0x75,0xb1,0x9e,0x7e,0xd2,0x39,0x5c,0x0e,
	0x58,0xba,0x17,0x14,0x62,0x8c,0x39,0xe1,0x18,0x50,0x8b,0xff,
	0xad,0xae,0x8e,0x6d,0x39,0x87,0x10,0x8e,0x44,0x9e,0x6d,0xf6,
	0xdd,0x0e,0xc5,0xe5,0xc9,0x3f,0xb4,0xcb,0x6a,0xe0,0xf7,0xec,
	0xd1,0xbf,0x1f,0x9e,0x2a,0x65,0x24,0x9c,0xb3,0x9b,0x30,0x55,
	0x6c,0x75,0x33,0xc8,0xd6,0xc5,0x90,0xab,0xa9,0x4e,0x94,0x73,
	0xb1,0x2e,0x0b,0xab,0xd7,0x3a,0x85,0xac,0xac,0x43,0x45,0x67,
	0xad,0xb8,0xc5,0x3d,0xbf,0x6d,0x99,0xef,0x67,0x36,0x35,0xb4,
	0x39,0xfe,0xf9,0x48,0x22,0xb6,0x09,0x6a,0xf2,0xdf,0x86,0x3e,
	0x4f,0xf7,0xa8,0x54,0x1d,0xa3,0xf3,0x17,0x0a,0xc0,0x66,0x12,
	0x68,0xd5,0x06,0x6b,0x23,0x20,0x0f,0xc2,0xc3,0x3c,0x6c,0xa3,
	0xa6,0xf7,0xc4,0x9a,0xd4,0x79,0x62,0xed,0x5b,0x86,0x7f,0x7b,
	0xdc,0x35,0x99,0x75,0xb5,0xc2,0xf7,0x09,0x1f,0xd5,0x8f,0x21,
	0xc5,0xc0,0xd3,0x59,0x21,0x36,0xf8,0x7b,0xb2,0x6d,0x01,0xc3,
	0x5d,0xe6,0x4d,0xf2,0x82,0xb2,0xe7,0x09,0x95,0xa6,0x5c,0x09,
	0xd5,0x88,0x05,0x45,0x8a,0x19,0x26,0x68,0x21,0xba,0xc6,0xe8,
	0x8f,0x22,0xa5,0x26,0xa2,0x9c,0xdf,0xfd,0xcf,0xf4,0xfe,0xb8,
	0xfa,0x0c,0x72,0x9c,0x67,0x4c,0x8a,0x89,0x35,0xb1,0xbb,0x41,
	0xf6,0x42,0xc9,0xff,0x95,0xfb,0x91,0x47,0x33,0x85,0xd1,0xd5,
	0x1c,0xa5,0xbc,0x0c,0xf9,0xd1,0xab,0xf8,0x41,0x71,0x53,0xaa,
	0xca,0x4e,0xc7,0xf3,0xf5,0x68,0x5c,0x2e,0xf2,0x62,0xd3,0x80,
	0x1a,0x89,0xc1,0x5d,0x21,0xfc,0xba,0x39,0xea,0x7c,0x7b,0xba,
	0x4f,0x70,0xc3,0x7a,0x3d,0xe0,0xce,0xc5,0xe8,0xfd,0x18,0xd3,
	0x47,0x0e,0xf3,0x98,0x42,0x3b,0xe8,0x03,0xcc,0x2e,0xd6,0xe7,
	0xe4,0x36,0x53,0x47,0xe6,0xf8,0xd0,0x09,0xc0,0x06,0xa9,0x23,
	0x10,0xc6,0xd3,0x4f,0xf6,0x8d,0x27,0xc3,0xbe,0xa0,0xc1,0x80,
	0x8b,0x2d,0x43,0x42,0x1a,0x5c,0xbd,0xbe,0xf3,0x14,0x6b,0x7e,
	0x44,0xa9,0x59,0x1b,0x2c,0x06,0xe4,0xfe,0xb7,0x44,0xbc,0x80,
	0xdc,0x11,0x24,0xd5,0x40,0x5e,0x7e,0xc4,0xce,0x6a,0x6c,0xdc,
	0xda,0x4d,0x3a,0xbc,0xa2,0xbf,0xb9,0xf0,0x76,0xa7,0xb0,0x82,
	0x39,0xb1,0x2a,0xc6,0x7b,0x78,0xba,0x18,0xf5,0xb7,0x06,0x43,
	0xe8,0x1c,0x4b,0x89,0xb9,0x07,0xf7,0x5f,0x88,0xd8,0x7b,0xf1,
	0xf7,0xc4,0x72,0xd9,0xa9,0x3d,0x79,0x0a,0xfc,0x39,0x7d,0xe9,
	0xc9,0x26,0xf7,0x0f,0x22,0xe0,0x80,0x12,0x49,0x39,0x21,0xa4,
	0xaa,0x75,0xdc,0x83,0x7a,0x65,0x23,0xe5,0xd1,0x38,0x0e,0xc3,
	0x69,0xdd,0x82,0xe1,0xeb,0xca,0x84,0x5e,0x2f,0x10,0x72,0x99,
	0xdb,0x7d,0xea,0xa5,0xc5,0x59,0x7e,0xd9,0xe0,0xf3,0xa9,0x4d,
	0xdb,0xb9,0x1c,0x80,0x02,0x11,0xa5,0xd0,0xa3,0xaa,0x53,0x54,
	0xc5,0xce,0x22,0x7b,0xfa,0xa9,0x9b,0x8f,0xc0,0x90,0xd4,0xcd,
	0x39,0x0a,0x9f,0x17,0x03,0xe4,0xe1,0xcd,0x5b,0x5a,0x1c,0x46,
	0x88,0x58,0x9b,0x11,0x6a,0x8c,0x34,0x11,0x8d,0x65,0xd3,0xa2,
	0x5e,0x19,0xbc,0xb4,0x15,0x4e,0x33,0x10,0xe4,0x87,0x72,0x00,
	0xc4,0x00,0xa3,0x30,0x59,0x29,0xd9,0x21
};

/* Exponent of Public key */
static u32 PublicExp = 0x1000100;

/* Modulus */
static u8 Modulus[XSECURE_RSA_SIZE] = {
	0xf0,0x1c,0xde,0x7b,0xdb,0x26,0xcd,0xd7,0xa7,0xe3,0xfe,0x51,
	0x0b,0x82,0x03,0xdd,0x12,0xc7,0xc7,0x60,0x22,0x97,0x38,0xfe,
	0xad,0x79,0x8c,0x9d,0x3f,0x9d,0x87,0x0e,0xac,0x9c,0xee,0xb6,
	0x13,0xcc,0x8c,0x75,0x7f,0xcf,0x7a,0xf3,0xe6,0x90,0x5e,0x59,
	0xb2,0x09,0xd9,0x75,0x94,0xd4,0x92,0x8d,0xc5,0x81,0xfa,0xf8,
	0x6e,0x63,0xf7,0x70,0x19,0xf3,0x31,0xd2,0xf3,0x28,0x6c,0x8a,
	0x79,0xca,0x34,0x03,0x4f,0x39,0x7f,0x09,0xf7,0x21,0x20,0xd6,
	0xfd,0x36,0xb3,0xa2,0x7e,0x40,0x96,0x3e,0x68,0x12,0x80,0xfe,
	0x92,0x37,0xe7,0xd5,0xdc,0xfe,0x62,0x5c,0x3e,0xce,0x21,0x71,
	0x70,0x03,0xd6,0xbd,0x02,0xd0,0x24,0x41,0xb6,0xfc,0x5c,0xfb,
	0x8e,0xf1,0xfa,0xc7,0x3d,0x9b,0x81,0xd7,0xee,0x0a,0x9f,0x4c,
	0x4b,0x81,0x9c,0x6d,0xe2,0x49,0xa5,0x98,0xaf,0xa0,0xc1,0x19,
	0x5c,0x0b,0x85,0x66,0x31,0x60,0x6c,0x44,0x77,0xe9,0x96,0x58,
	0xac,0x1d,0x72,0x9e,0xcd,0xb2,0xc2,0x6a,0x5a,0x90,0xeb,0xe9,
	0x30,0x52,0x97,0x3f,0xd1,0x9b,0xb6,0xac,0x17,0x84,0xfc,0xa7,
	0xd2,0x64,0x7a,0xa7,0x40,0x90,0xac,0xb9,0x1a,0x96,0x28,0xa0,
	0xf4,0xb0,0xfb,0x07,0x16,0x26,0x29,0x3d,0x9f,0xac,0xe7,0x2e,
	0xe8,0x72,0x79,0xf9,0x5e,0x14,0x33,0xed,0xe1,0xc2,0x51,0x03,
	0xca,0xf2,0xdc,0xa8,0x49,0x41,0x2c,0x29,0x13,0x59,0xec,0xad,
	0x95,0x34,0xe3,0x5f,0xcd,0x35,0xc5,0x60,0xaf,0xe6,0x12,0x21,
	0xcc,0x99,0x85,0x13,0x78,0xc1,0xce,0x1a,0x26,0xfc,0x34,0x26,
	0x3d,0x9a,0xfd,0xfe,0x5e,0x50,0xba,0xe7,0x9b,0x97,0x63,0xb2,
	0x3d,0xf9,0xf5,0xdd,0x52,0xb7,0xb5,0x0f,0x6e,0x1b,0x1e,0x12,
	0xb6,0x5b,0xa7,0xf4,0xb6,0x48,0xc5,0x5f,0x72,0x3b,0xc3,0x46,
	0x7b,0x30,0xef,0xe8,0x37,0xd9,0xba,0x34,0x77,0xc5,0x72,0xb5,
	0xf8,0xa3,0x56,0x4a,0xc7,0xdb,0x29,0xab,0x11,0x89,0x0a,0xe5,
	0x94,0xcf,0x60,0x8c,0x97,0x68,0xb2,0x4e,0x8b,0x49,0xc1,0xea,
	0x7f,0x74,0x35,0x29,0xa1,0xa3,0xf9,0xd4,0xe5,0xa0,0x79,0x25,
	0xcb,0x2e,0x5b,0x83,0xed,0x0a,0xb3,0x36,0x0c,0x0e,0x2e,0xda,
	0x49,0xf9,0x94,0x2e,0xe3,0xd2,0x62,0xa1,0xf8,0xdd,0xf8,0x77,
	0xb3,0xf6,0xd8,0x05,0xd8,0x9a,0x8f,0x65,0x31,0x67,0xa7,0x6d,
	0x11,0xa3,0xee,0xcd,0xd3,0x79,0x0a,0x46,0xd2,0x4a,0xe0,0xdd,
	0x02,0x09,0xf4,0x5f,0x41,0x49,0x02,0x3e,0x05,0xd1,0x11,0x72,
	0x46,0xff,0xbd,0x7d,0x75,0xb1,0x5f,0xa3,0x3b,0xa6,0xf2,0xcd,
	0x24,0xe3,0xca,0x2d,0xd6,0xfc,0x94,0xdd,0x9e,0x6f,0x13,0xa6,
	0xb0,0xa9,0x88,0x2e,0x14,0x74,0xd2,0xc3,0xa7,0xe1,0xa5,0xae,
	0x6d,0x7a,0x06,0x8f,0x48,0x95,0x08,0x84,0x50,0x6c,0xc4,0xa6,
	0xfd,0xc6,0x3a,0x25,0x03,0x8e,0x19,0xb8,0x82,0x88,0xad,0xee,
	0xbc,0x8d,0xbc,0xf1,0xdd,0xf7,0x1a,0x4c,0x2e,0x1d,0x5d,0x54,
	0x82,0x75,0x35,0x7c,0xd5,0xe3,0x25,0x40,0x8b,0x4a,0xa5,0x49,
	0x4e,0x52,0x14,0x1f,0xc5,0xd8,0xb8,0xef,0xae,0xcf,0x50,0xbb,
	0x9d,0xbc,0xe7,0x93,0x71,0x04,0xf8,0xf4,0xa6,0xf1,0xe2,0x73,
	0xf8,0x56,0x00,0x30,0xba,0xee,0xd5,0xe7
};

/* Hash with PKCS padding */
/*
 * MSB  ------------------------------------------------------------LSB
 * 0x0 || 0x1 || 0xFF(for 202 bytes) || 0x0 || T_padding || SHA384 Hash
 */
static u8 Data[XSECURE_RSA_SIZE] = {
	 0x00,0x01,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	 0x00,
	 /* T_Padding */
	 0x30,0x41,0x30,0x0D,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,
	 0x04,0x02,0x09,0x05,0x00,0x04,0x30,
	 /* SHA 3 Hash */
	 0x85,0x52,0xA4,0xA3,0xC3,0x06,0x84,0x9C,0x00,0x22,0x43,0x10,
	 0x78,0x15,0xD7,0xD9,0x2D,0xF6,0x5F,0x07,0x35,0xD5,0x45,0x80,
	 0x35,0xE1,0xA7,0x9C,0xBB,0xBE,0x98,0x02,0x89,0x0D,0x7C,0x4D,
	 0xC0,0x63,0x24,0x01,0x21,0x5F,0xC5,0x9D,0xBC,0xAE,0x00,0xD3
};

static u8 ExpectedSign[XSECURE_RSA_SIZE] = {
	 0xCE,0x8F,0x94,0xA6,0xD0,0xB4,0x4A,0x74,0x1B,0xA0,0xCE,0x17,
	 0x56,0x2E,0xB3,0xBF,0x45,0x00,0x02,0x73,0x5C,0x23,0x36,0x9D,
	 0x58,0xB4,0x76,0x8A,0xD4,0xCF,0xCF,0xF1,0xAC,0x96,0x3F,0xEE,
	 0xB2,0x62,0x55,0x09,0xBA,0x22,0x5E,0x86,0xA2,0xD0,0xE5,0x01,
	 0xAA,0x48,0xF1,0x2A,0x49,0xCD,0xB3,0x24,0xB3,0x5C,0xD6,0x7A,
	 0x98,0x0E,0xDC,0xC1,0x95,0xB7,0xAA,0x4D,0x26,0xEF,0x88,0xE3,
	 0x13,0xC3,0x03,0x37,0x4F,0x7C,0xFA,0xF5,0x40,0x99,0x36,0x88,
	 0xE8,0x11,0xFA,0x42,0x8A,0xA8,0x14,0xF4,0xCC,0x2F,0xE1,0x29,
	 0xAA,0xBB,0x07,0x17,0x68,0x21,0xFC,0x1D,0xC4,0x0A,0xFF,0x93,
	 0x8F,0x43,0x0A,0x40,0x54,0xED,0xD3,0xDC,0xA0,0x57,0x7C,0xD8,
	 0xB4,0xD9,0x03,0x05,0x51,0xFF,0xFB,0x0A,0x90,0x95,0xD7,0x34,
	 0x93,0x39,0x65,0x0F,0xE6,0xC1,0x3F,0x60,0x15,0xE5,0x9B,0xC2,
	 0x9F,0x52,0x3F,0xA6,0x29,0x6A,0xC8,0x02,0x21,0x2A,0xD9,0xD0,
	 0x6F,0xA9,0x5F,0x2D,0x4F,0xBD,0xCA,0x14,0xC1,0xAD,0x9B,0x23,
	 0xF1,0x21,0xF6,0xEE,0x51,0xB6,0x40,0x04,0x6D,0x01,0x48,0x19,
	 0x13,0x76,0x6B,0xF5,0x22,0x4A,0x3D,0x53,0x0D,0xF3,0xE6,0x7C,
	 0xB0,0xDA,0xE0,0x61,0x8C,0x21,0x7A,0x08,0xFF,0xF9,0xE3,0xF4,
	 0xC4,0x4D,0x59,0x83,0xCB,0xC9,0xED,0xA5,0xD0,0x3D,0xC8,0x05,
	 0xD7,0xA1,0x6C,0xFF,0xE7,0xD8,0xB9,0x5C,0xAE,0x7D,0x03,0xE3,
	 0x21,0xDB,0xFB,0x06,0x4B,0x71,0xE9,0x1C,0x3F,0xBB,0xC4,0xBB,
	 0x03,0xF5,0x6D,0x1B,0xC3,0xB0,0x2D,0x15,0xAC,0x1D,0xCA,0x71,
	 0x89,0x8D,0x21,0xD5,0x68,0x1A,0x17,0xCA,0x01,0x6B,0x01,0x2A,
	 0xC3,0xA2,0x01,0x37,0x54,0x52,0xE2,0xED,0xE8,0xC9,0x33,0xCA,
	 0xE1,0x81,0xE3,0x7B,0x4D,0xDB,0x9F,0xF1,0xF0,0x9E,0x01,0xBD,
	 0x51,0x65,0xAE,0x14,0x57,0xBA,0x0E,0x3C,0x6C,0x82,0x8C,0x8E,
	 0x4E,0xF3,0x51,0x99,0x8B,0x7F,0x96,0x3C,0xD1,0x4E,0x6E,0x38,
	 0xD0,0x5F,0x3B,0x0D,0x8F,0x45,0xFD,0x8D,0x12,0x9B,0x0A,0x53,
	 0x34,0x50,0x1E,0x86,0x9E,0x89,0xE1,0xD4,0x58,0x60,0x68,0xDF,
	 0x53,0x15,0x36,0x82,0x3B,0xC3,0x39,0x8F,0xFE,0x1B,0xD2,0xBA,
	 0x3A,0xBD,0xE5,0x63,0x45,0xE1,0xC7,0x92,0x0C,0xCF,0xE3,0x1D,
	 0x93,0x01,0x55,0xA4,0x05,0x28,0xA8,0xBB,0xE2,0xFD,0xCA,0x00,
	 0xAC,0x53,0x97,0xE0,0x17,0xC6,0x5D,0x18,0x7A,0x1F,0xBD,0x51,
	 0xD9,0xDF,0xA1,0xAC,0x7C,0x91,0xA9,0x3A,0xF9,0x28,0x6F,0xA6,
	 0xB9,0x65,0x0B,0xAF,0xC2,0xB9,0x46,0xBE,0x25,0x31,0x18,0x83,
	 0x25,0x10,0x76,0x87,0x2A,0x94,0xEC,0xBC,0x1A,0xBC,0x25,0x72,
	 0x56,0xA6,0xB0,0x66,0xE1,0x7B,0x4F,0xC7,0xCE,0x7D,0x4D,0x1C,
	 0x12,0x50,0xDB,0x89,0x0D,0xEF,0xC0,0x73,0x7B,0x47,0xA2,0xC2,
	 0x66,0x9A,0x8A,0xF2,0xE0,0x9D,0x04,0x73,0xAB,0xC3,0x0C,0x3E,
	 0x3E,0x66,0x55,0xEE,0x3C,0xAA,0xC1,0xE3,0xB0,0x4A,0xC1,0xBD,
	 0xAA,0xBA,0x10,0xCD,0x4B,0x15,0xF3,0x8F,0x59,0x81,0x54,0xE6,
	 0x63,0x3C,0xC7,0xA9,0xB1,0x40,0x1A,0x7B,0x8A,0x22,0xD1,0x10,
	 0xC5,0x7C,0x3F,0xF5,0xAB,0xBE,0x5E,0x4C,0x14,0xD6,0x83,0x6E,
	 0x31,0x6A,0x45,0x5F,0x4A,0x93,0xE5,0x13
};

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

static u32 SecureRsaExample(void);

/************************** Variable Definitions *****************************/

static XSecure_Rsa Secure_Rsa;
static u8 Signature[XSECURE_RSA_SIZE];
static u8 EncryptSignatureOut[XSECURE_RSA_SIZE];
static u32 Size = XSECURE_RSA_SIZE;

/*****************************************************************************/
/**
*
* This example illustrates the usage of RSA APIs to encrypt or decrypt the
* signature
*
* @param	None
*
* @return
*		- XST_FAILURE if the boot image authentication Failed .
*
* @note		None
*
******************************************************************************/
int main(void)
{
	u32 Status;

	Status = SecureRsaExample();

	if(Status != XST_SUCCESS)
	{
		xil_printf("\r\nGeneric RSA example failed %d \r\n",
					Status);
		return XST_FAILURE;
	}

	xil_printf("\r\nSuccessfully ran Generic RSA example\r\n ");

	return XST_SUCCESS;
}

/****************************************************************************/
/**
*
* This function authenticates boot image located in DDR using RSA-4096
* algorithm. The decrypted hash is matched against the calculated boot image
* hash.
* The purpose of this function is to illustrate how to use the XSecure_Rsa
* driver.
*
*
* @return	- XST_FAILURE if example is failed
*		- XST_SUCCESS on success.
*
* @note		None.
*
****************************************************************************/
/** //! [RSA generic example] */
static u32 SecureRsaExample(void)
{
	u32 Index;

	/* RSA signature decrypt with private key */
	/*
	 * Initialize the Rsa driver with private key components
	 * so that it's ready to use
	 */
	XSecure_RsaInitialize(&Secure_Rsa, Modulus, NULL, PrivateExp);


	if(XST_SUCCESS != XSecure_RsaPrivateDecrypt(&Secure_Rsa, Data,
						Size, Signature))	{
		xil_printf("Failed at RSA signature decryption\n\r");
		return XST_FAILURE;
	}

	xil_printf("\r\n Decrypted Signature with private key\r\n ");

	for(Index = 0; Index < Size; Index++) {
		xil_printf(" %02x ", Signature[Index]);
	}
	xil_printf(" \r\n ");

	/* Verification if Data is expected */
	for(Index = 0; Index < Size; Index++) {
		if (Signature[Index] != ExpectedSign[Index]) {
			xil_printf("\r\nError at verification of RSA signature"
					" Decryption\n\r");
			return XST_FAILURE;
		}
	}

	/* RSA signature encrypt with Public key components */

	/*
	 * Initialize the Rsa driver with public key components
	 * so that it's ready to use
	 */

	XSecure_RsaInitialize(&Secure_Rsa, Modulus, NULL, (u8 *)&PublicExp);

	if(XST_SUCCESS != XSecure_RsaPublicEncrypt(&Secure_Rsa, Signature,
						Size, EncryptSignatureOut))	{
		xil_printf("\r\nFailed at RSA signature encryption\n\r");
		return XST_FAILURE;
	}
	xil_printf("\r\n Encrypted Signature with public key\r\n ");

	for(Index = 0; Index < Size; Index++) {
		xil_printf(" %02x ", EncryptSignatureOut[Index]);
	}

	/* Verification if Data is expected */
	for(Index = 0; Index < Size; Index++) {
		if (EncryptSignatureOut[Index] != Data[Index]) {
			xil_printf("\r\nError at verification of RSA signature"
					" encryption\n\r");
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}
/** //! [RSA generic example] */
/** @} */
