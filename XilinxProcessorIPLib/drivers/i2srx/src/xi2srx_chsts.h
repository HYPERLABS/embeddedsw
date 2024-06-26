/******************************************************************************
* Copyright (C) 2017 - 2020 Xilinx, Inc. All rights reserved.
* Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
 * ****************************************************************************/

/*****************************************************************************/
/*
 * @file xi2srx_chsts.h
 * @addtogroup i2srx Overview
 * @{
 * Format status related offsets & masks definitions related to the channel
 * status format.For formats/line protocols check the AES standard
 * specifications document
 * <pre>
 * MODIFICATION HISTORY:
 * Ver   Who    Date      Changes
 * ----- ------ -------- --------------------------------------------------
 * 1.0   kar    01/25/18  Initial release.
 *
 * </pre>
 *****************************************************************************/
#ifndef XI2SRX_CHSTS_H
#define XI2SRX_CHSTS_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xi2srx.h"

/************************** Constant Definitions *****************************/
/** @name AES Status and Register Masks and Shifts @{ */

#define XI2S_RX_AES_STS_USE_OF_CH_STS_BLK_SHIFT (0)
	/**< Use of Channel Status Block bit shift */

#define XI2S_RX_AES_STS_LINEAR_PCM_ID_SHIFT (1)
	/**< Linear PCM Identification bit shift */

#define XI2S_RX_AES_STS_AUDIO_SIG_PRE_EMPH_SHIFT (2)
	/**< Audio signal pre-emphasis bit shift */

#define XI2S_RX_AES_STS_LOCK_INDICATION_SHIFT (5)
	/**< lock indication bit shift */

#define XI2S_RX_AES_STS_SAMPLING_FREQ_E_SHIFT (6)
	/**<Sampling Frequency 0 bit shift */

#define XI2S_RX_AES_STS_CH_MODE_SHIFT (0)
	/**< Channel mode bit shift */

#define XI2S_RX_AES_STS_USR_BITS_MGMT_SHIFT (4)
	/**< User Bits Management bit shift */

#define XI2S_RX_AES_STS_USEOF_AUX_SMPL_BITS_SHIFT (0)
	/**< Use of auxiliary sample bits bit shift */

#define XI2S_RX_AES_STS_SRC_WORD_LENGTH_SHIFT (3)
	/**< Source word length bit shift */

#define XI2S_RX_AES_STS_INDICATE_ALIGN_LEVEL_SHIFT (6)
	/**< Indication of Alignment level bit shift */

#define XI2S_RX_AES_STS_CH_NUM0_SHIFT (0)
	/**<Channel Number (0) bit shift */

#define XI2S_RX_AES_STS_MC_CH_MODE_SHIFT (7)
	/**< Multi-channel mode bit shift*/

#define XI2S_RX_AES_STS_CH_NUM1_SHIFT (0)
	/**< Channel Number (1) bit shift */

#define XI2S_RX_AES_STS_MC_CH_MODE_NUM_SHIFT (4)
	/**< Multi-channel mode number bit shift */

#define XI2S_RX_AES_STS_RSVD_BUT_UNDEF0_SHIFT (2)
	/**< Reserved but undefined (0) bit shift */

#define XI2S_RX_AES_STS_SAMPLING_FREQ_Q_SHIFT (3)
	/**< Sampling Frequency (1) bit shift */

#define XI2S_RX_AES_STS_SAMPLING_FREQ_SCALE_FLAG_SHIFT (7)
	/**< Sampling Frequency scaling flag bit shift */

#define XI2S_RX_AES_STS_RSVD_BUT_UNDEF1_SHIFT (0)
	/**< Reserved but undefined (1) bit shift */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTEO_MASK (0XFF)
	/**< Alphanumeric channel origin data register(s) byte0 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTE1_MASK (0XFF00)
	/**< Alphanumeric channel origin data register(s) byte1 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTE1_SHIFT (8)
	/**< Alphanumeric channel origin data register(s) byte1 shift */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTE2_MASK (0XFF0000)
	/**< Alphanumeric channel origin data register(s) byte2 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTE2_SHIFT (16)
	/**< Alphanumeric channel origin data egister(s) byte2 shift */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTE3_MASK (0XFF000000)
	/**< Alphanumeric channel origin data register(s) byte3 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_ORG_DATA_BYTE3_SHIFT (24)
	/**< Alphanumeric channel origin data register(s) byte3 shift */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTEO_MASK (0XFF)
	/**< Alphanumeric channel destination data register(s) byte0 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTE1_MASK (0XFF00)
	/**< Alphanumeric channel  destination data register(s) byte1 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTE1_SHIFT (8)
	/**< Alphanumeric channel destination data register(s) byte1 shift */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTE2_MASK (0XFF0000)
	/**< Alphanumeric channel  destination data register(s) byte2 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTE2_SHIFT (16)
	/**< Alphanumeric channel destination data register(s) byte2 shift */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTE3_MASK (0XFF000000)
	/**< Alphanumeric channel  destination data register(s) byte3 mask */

#define XI2S_RX_AES_STS_ALPHANUM_CH_DEST_DATA_BYTE3_SHIFT (24)
	/**< Alphanumeric channel destination data register(s) byte3 shift */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTEO_MASK (0XFF)
	/**< Local sample address code register(s) byte0 mask */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTE1_MASK (0XFF00)
	/**< Local sample address code register(s) byte1 mask */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTE1_SHIFT (8)
	/**< Local sample address code register(s) byte1 shift */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTE2_MASK (0XFF0000)
	/**< Local sample address code register(s) byte2 mask */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTE2_SHIFT (16)
	/**< Local sample address code register(s) byte2 shift */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTE3_MASK (0XFF000000)
	/**< Local sample address code register(s) byte3 mask */

#define XI2S_RX_AES_STS_LOCAL_SAMPLE_ADDRESS_CODE_BYTE3_SHIFT (24)
	/**< Local sample address code register(s) byte3 shift */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTEO_MASK (0XFF)
	/**< Time-of-day sample address code register(s) byte0 mask */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTE1_MASK (0XFF00)
	/**< Time-of-day sample address code register(s) byte1 mask */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTE1_SHIFT (8)
	/**< Time-of-day sample address code register(s) byte1 shift */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTE2_MASK (0XFF0000)
	/**< Time-of-day sample address code register(s) byte2 mask */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTE2_SHIFT (16)
	/**< Time-of-day sample address code register(s) byte2 shift */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTE3_MASK (0XFF000000)
	/**< Time-of-day sample address code register(s) byte3 mask */

#define XI2S_RX_AES_STS_TIME_OF_DAY_SAMPLE_ADDRESS_CODE_BYTE3_SHIFT (24)
	/*< Time-of-day sample address code register(s) byte3 shift */

/** @} */
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void XI2s_Rx_SetAesChStat(u32 I2srx_SrcBuf[], u8 I2srx_DstBuf[]);

/************************** Variable Definitions *****************************/

/************************** Function Definitions *****************************/

#ifdef __cplusplus
}
#endif

#endif /*XI2SRX_CHSTS_H*/
/** @} */
