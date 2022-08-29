/******************************************************************************
* Copyright (c) 2022 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/******************************************************************************/
/**
*
* @file xnvm_efuse_hw.h
*
* This file contains Versal_Net NVM library eFUSE controller register
* definitions
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- ---------- --------------------------------------------------------
* 1.0   kal  12/07/2022 Initial release
*
* </pre>
*
* @note
*
*******************************************************************************/
#ifndef XNVM_EFUSE_HW_H
#define XNVM_EFUSE_HW_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Include Files *********************************/

/*************************** Constant Definitions *****************************/
/**< EFUSE row count numbers */
#define XNVM_EFUSE_PPK_HASH_NUM_OF_CACHE_ROWS		(8U)
#define XNVM_EFUSE_TOTAL_PPK_HASH_ROWS			(XNVM_EFUSE_PPK_HASH_NUM_OF_CACHE_ROWS * 3U)
#define XNVM_EFUSE_IV_NUM_OF_CACHE_ROWS			(3U)
/** @} */

/**< EFUSE Row numbers */
#define XNVM_EFUSE_TBITS_XILINX_CTRL_ROW		(0U)
#define XNVM_EFUSE_PUF_RO_SWAP_EN_ROW                   (1U)
#define XNVM_EFUSE_BLACK_IV_START_ROW			(4U)
#define XNVM_EFUSE_PLM_IV_START_ROW			(4U)
#define XNVM_EFUSE_DATA_PARTITION_IV_START_ROW		(4U)
#define XNVM_EFUSE_AES_KEY_0_TO_127_START_ROW		(16U)
#define XNVM_EFUSE_AES_KEY_128_TO_255_START_ROW		(16U)
#define XNVM_EFUSE_BOOT_MODE_START_ROW			(16U)
#define XNVM_EFUSE_MISC_CTRL_START_ROW			(20U)
#define XNVM_EFUSE_ANLG_TRIM_3_START_ROW		(24U)
#define XNVM_EFUSE_GLITCH_DET_WR_LK_ROW			(27U)
#define XNVM_EFUSE_BOOT_ENV_CTRL_START_ROW		(28U)
#define XNVM_EFUSE_AES_KEY_0_TO_255_END_ROW		(31U)
#define XNVM_EFUSE_REVOKE_ID_0_TO_127_START_ROW		(32U)
#define XNVM_EFUSE_REVOKE_ID_128_TO_255_START_ROW	(32U)
#define XNVM_EFUSE_SECURITY_MISC_0_START_ROW		(40U)
#define XNVM_EFUSE_SEC_CTRL_START_ROW			(44U)
#define XNVM_EFUSE_SECURITY_MISC1_START_ROW		(48U)
#define XNVM_EFUSE_DICE_UDS_0_TO_63_START_ROW		(48U)
#define XNVM_EFUSE_USER_KEY1_0_TO_63_START_ROW		(48U)
#define XNVM_EFUSE_DME_REVOKE_0_AND_1_ROW		(52U)
#define XNVM_EFUSE_DME_MODE_START_ROW			(52U)
#define XNVM_EFUSE_DME_REVOKE_2_AND_3_ROW		(53U)
#define XNVM_EFUSE_USER_KEY1_0_TO_63_END_ROW		(55U)
#define XNVM_EFUSE_DME_FIPS_ROW				(55U)
#define XNVM_EFUSE_DICE_UDS_64_TO_191_START_ROW		(56U)
#define XNVM_EFUSE_USER_KEY0_0_TO_63_START_ROW		(56U)
#define XNVM_EFUSE_USER_KEY0_0_TO_63_END_ROW		(63U)
#define XNVM_EFUSE_DICE_UDS_192_TO_255_START_ROW	(64U)
#define	XNVM_EFUSE_DICE_UDS_256_TO_383_START_ROW	(66U)
#define XNVM_EFUSE_USER_KEY0_64_TO_191_START_ROW	(66U)
#define XNVM_EFUSE_USER_KEY1_64_TO_127_START_ROW	(66U)
#define XNVM_EFUSE_USER_KEY0_64_TO_191_END_ROW		(73U)
#define XNVM_EFUSE_USER_KEY1_64_TO_127_END_ROW		(73U)
#define XNVM_EFUSE_USER_KEY0_192_TO_255_START_ROW	(74U)
#define XNVM_EFUSE_USER_KEY1_128_TO_255_START_ROW	(74U)
#define XNVM_EFUSE_USER_KEY0_192_TO_255_END_ROW		(81U)
#define XNVM_EFUSE_USER_KEY1_128_TO_255_END_ROW		(81U)
#define XNVM_EFUSE_OFFCHIP_REVOKE_ID_START_ROW		(82U)
#define XNVM_EFUSE_META_HEADER_IV_START_ROW		(90U)
#define XNVM_EFUSE_PUF_CHASH_ROW			(93U)
#define XNVM_EFUSE_IP_DISABLE_ROW			(94U)
#define XNVM_EFUSE_PUF_AUX_ROW				(95U)
#define XNVM_EFUSE_PPK1_HASH_START_ROW			(96U)
#define XNVM_EFUSE_DME_USER_KEY_0_START_ROW		(96U)
#define XNVM_EFUSE_DME_USER_KEY_2_START_ROW		(96U)
#define XNVM_EFUSE_PPK2_HASH_START_ROW			(128U)
#define XNVM_EFUSE_DME_USER_KEY_1_START_ROW		(144U)
#define XNVM_EFUSE_DME_USER_KEY_3_START_ROW		(144U)
#define XNVM_EFUSE_PPK0_HASH_START_ROW			(160U)
#define XNVM_EFUSE_CRC_SALT_ROW				(179U)
#define XNVM_EFUSE_DISABLE_PLM_UPDATE_ROW		(185U)
#define XNVM_EFUSE_CRC_ROW				(188U)
#define XNVM_EFUSE_PAGE_0_PUF_SYN_DATA_ROW		(192U)
#define XNVM_EFUSE_PAGE_1_PUF_SYN_DATA_ROW		(192U)


#define XNVM_EFUSE_ROW_0_SEC_CTRL_PROT_0_COL_NUM	(2U)
#define XNVM_EFUSE_ROW_0_SEC_CTRL_PROT_1_COL_NUM	(25U)
#define XNVM_EFUSE_ROW_0_SEC_MISC0_PROT_1_COL_NUM	(24U)
#define XNVM_EFUSE_ROW_0_SEC_MISC0_PROT_0_COL_NUM	(3U)
#define XNVM_EFUSE_ROW_0_PPK_HASH_PROT_0_COL_NUM	(16U)
#define XNVM_EFUSE_ROW_0_PPK_HASH_PROT_1_COL_NUM	(23U)
#define XNVM_EFUSE_ROW_0_META_HEADER_EXPORT_DFT_PROT_0_COL_NUM	(17U)
#define XNVM_EFUSE_ROW_0_META_HEADER_EXPORT_DFT_PROT_1_COL_NUM	(22U)
#define XNVM_EFUSE_ROW_0_CRC_PROT_0_COL_NUM		(21U)
#define XNVM_EFUSE_ROW_0_CRC_PROT_1_COL_NUM		(20U)
#define XNVM_EFUSE_ROW_0_PUF_CHASH_PROT_COL_NUM		(19U)
#define XNVM_EFUSE_ROW_0_SEC_MISC1_PROT_COL_NUM		(18U)
#define XNVM_EFUSE_AES_KEY_0_TO_127_COL_START_NUM	(8U)
#define XNVM_EFUSE_AES_KEY_0_TO_127_COL_END_NUM		(15U)
#define XNVM_EFUSE_AES_KEY_128_TO_255_COL_START_NUM	(16U)
#define XNVM_EFUSE_AES_KEY_128_TO_255_COL_END_NUM	(23U)
#define XNVM_EFUSE_USER_KEY0_0_TO_63_COL_START_NUM	(8U)
#define XNVM_EFUSE_USER_KEY0_0_TO_63_COL_END_NUM	(15U)
#define XNVM_EFUSE_USER_KEY0_64_TO_191_COL_START_NUM	(8U)
#define XNVM_EFUSE_USER_KEY0_64_TO_191_COL_END_NUM	(23U)
#define XNVM_EFUSE_USER_KEY0_192_TO_255_COL_START_NUM	(8U)
#define XNVM_EFUSE_USER_KEY0_192_TO_255_COL_END_NUM	(15U)
#define XNVM_EFUSE_USER_KEY1_0_TO_63_START_COL_NUM	(16U)
#define XNVM_EFUSE_USER_KEY1_0_TO_63_END_COL_NUM	(23U)
#define XNVM_EFUSE_USER_KEY1_64_TO_127_START_COL_NUM	(24U)
#define XNVM_EFUSE_USER_KEY1_64_TO_127_END_COL_NUM	(31U)
#define XNVM_EFUSE_USER_KEY1_128_TO_255_START_COL_NUM	(16U)
#define XNVM_EFUSE_USER_KEY1_128_TO_255_END_COL_NUM	(31U)
#define XNVM_EFUSE_PPK0_HASH_START_COL_NUM		(16U)
#define XNVM_EFUSE_PPK0_HASH_END_COL_NUM		(23U)
#define XNVM_EFUSE_PPK1_HASH_START_COL_NUM		(24U)
#define XNVM_EFUSE_PPK1_HASH_END_COL_NUM		(31U)
#define XNVM_EFUSE_PPK2_HASH_START_COL_NUM              (24U)
#define XNVM_EFUSE_PPK2_HASH_END_COL_NUM                (31U)
#define XNVM_EFUSE_METAHEADER_IV_RANGE_START_COL_NUM	(0U)
#define XNVM_EFUSE_METAHEADER_IV_RANGE_END_COL_NUM	(31U)
#define XNVM_EFUSE_BLACK_IV_START_COL_NUM		(8U)
#define XNVM_EFUSE_BLACK_IV_END_COL_NUM			(15U)
#define XNVM_EFUSE_PLM_IV_RANGE_START_COL_NUM		(16U)
#define XNVM_EFUSE_PLM_IV_RANGE_END_COL_NUM		(23U)
#define XNVM_EFUSE_DATA_PARTITION_IV_START_COL_NUM	(24U)
#define XNVM_EFUSE_DATA_PARTITION_IV_END_COL_NUM	(31U)
#define XNVM_EFUSE_REVOKE_ID_0_TO_127_START_COL_NUM	(16U)
#define XNVM_EFUSE_REVOKE_ID_128_TO_255_START_COL_NUM	(24U)
#define XNVM_EFUSE_MISC_CTRL_START_COL_NUM		(24U)
#define XNVM_EFUSE_MISC_CTRL_END_COL_NUM		(31U)
#define XNVM_EFUSE_SEC_CTRL_START_COL_NUM		(8U)
#define XNVM_EFUSE_SEC_CTRL_END_COL_NUM			(15U)
#define XNVM_EFUSE_GLITCH_DET_CONFIG_START_COL_NUM	(24U)
#define XNVM_EFUSE_GLITCH_DET_CONFIG_END_COL_NUM	(31U)
#define XNVM_EFUSE_GLITCH_DET_WR_LK_COL_NUM             (31U)
#define XNVM_EFUSE_DISABLE_PLM_UPDATE_COL_NUM		(26U)
#define XNVM_EFUSE_BOOT_MODE_START_COL_NUM		(24U)
#define XNVM_EFUSE_BOOT_MODE_END_COL_NUM		(31U)
#define XNVM_EFUSE_DME_MODE_START_COL_NUM		(8U)
#define XNVM_EFUSE_DME_MODE_END_COL_NUM			(11U)
#define XNVM_EFUSE_FIPS_MODE_START_COL_NUM		(8U)
#define XNVM_EFUSE_FIPS_MODE_END_COL_NUM		(15U)
#define XNVM_EFUSE_FIPS_VERSION_COL_0_NUM		(2U)
#define XNVM_EFUSE_FIPS_VERSION_COL_1_NUM		(30U)
#define XNVM_EFUSE_FIPS_VERSION_COL_2_NUM		(31U)
#define XNVM_EFUSE_PUF_SYN_DATA_START_COL_NUM		(0U)
#define XNVM_EFUSE_PUF_SYN_DATA_END_COL_NUM		(31U)
#define XNVM_EFUSE_PUF_CHASH_START_COL_NUM		(0U)
#define XNVM_EFUSE_PUF_CHASH_END_COL_NUM		(31U)
#define XNVM_EFUSE_PUF_AUX_START_COL_NUM		(0U)
#define XNVM_EFUSE_PUF_AUX_END_COL_NUM			(23U)
#define XNVM_EFUSE_PUF_REGIS_DIS_COL_NUM		(29U)
#define XNVM_EFUSE_PUF_HD_INVLD_COL_NUM			(30U)
#define XNVM_EFUSE_PUF_REGEN_DIS_COL_NUM		(31U)
#define XNVM_EFUSE_PUF_RO_SWAP_EN_START_COL_NUM		(0U)
#define XNVM_EFUSE_PUF_RO_SWAP_EN_END_COL_NUM		(31U)
#define XNVM_EFUSE_DEC_ONLY_START_COL_NUM		(8U)
#define XNVM_EFUSE_DEC_ONLY_END_COL_NUM			(15U)
#define	XNVM_EFUSE_SECURITY_MISC1_START_COL_NUM		(8U)
#define XNVM_EFUSE_SECURITY_MISC1_END_COL_NUM		(15U)
#define XNVM_EFUSE_BOOT_ENV_CTRL_START_COL_NUM		(24U)
#define XNVM_EFUSE_BOOT_ENV_CTRL_END_COL_NUM		(31U)
#define XNVM_EFUSE_DICE_UDS_0_TO_63_START_COL_NUM	(24U)
#define XNVM_EFUSE_DICE_UDS_0_TO_63_END_COL_NUM		(31U)
#define XNVM_EFUSE_DICE_UDS_64_TO_191_START_COL_NUM	(16U)
#define XNVM_EFUSE_DICE_UDS_64_TO_191_END_COL_NUM	(31U)
#define XNVM_EFUSE_DICE_UDS_192_TO_255_START_COL_NUM	(0U)
#define XNVM_EFUSE_DICE_UDS_192_TO_255_END_COL_NUM	(31U)
#define XNVM_EFUSE_DICE_UDS_256_TO_383_START_COL_NUM	(0U)
#define XNVM_EFUSE_DICE_UDS_256_TO_383_END_COL_NUM	(7U)
#define XNVM_EFUSE_DME_REVOKE_0_0_COL_NUM		(12U)
#define XNVM_EFUSE_DME_REVOKE_0_1_COL_NUM		(13U)
#define XNVM_EFUSE_DME_REVOKE_1_0_COL_NUM		(14U)
#define XNVM_EFUSE_DME_REVOKE_1_1_COL_NUM		(15U)
#define XNVM_EFUSE_DME_REVOKE_2_0_COL_NUM		(8U)
#define XNVM_EFUSE_DME_REVOKE_2_1_COL_NUM		(9U)
#define XNVM_EFUSE_DME_REVOKE_3_0_COL_NUM		(10U)
#define XNVM_EFUSE_DME_REVOKE_3_1_COL_NUM		(11U)
#define XNVM_EFUSE_DME_USER_KEY_0_START_COL_NUM		(0U)
#define XNVM_EFUSE_DME_USER_KEY_0_END_COL_NUM		(7U)
#define XNVM_EFUSE_DME_USER_KEY_1_START_COL_NUM		(0U)
#define XNVM_EFUSE_DME_USER_KEY_1_END_COL_NUM		(7U)
#define XNVM_EFUSE_DME_USER_KEY_2_START_COL_NUM		(8U)
#define XNVM_EFUSE_DME_USER_KEY_2_END_COL_NUM		(15U)
#define XNVM_EFUSE_DME_USER_KEY_3_START_COL_NUM		(8U)
#define XNVM_EFUSE_DME_USER_KEY_3_END_COL_NUM		(15U)
#define XNVM_EFUSE_CRC_START_COL_NUM			(24U)
#define XNVM_EFUSE_CRC_END_COL_NUM			(31U)
#define XNVM_EFUSE_CRC_SALT_START_COL_NUM		(24U)
#define XNVM_EFUSE_CRC_SALT_END_COL_NUM			(31U)

#define XNVM_EFUSE_AES_KEY_0_TO_127_NUM_OF_ROWS		(16U)
#define XNVM_EFUSE_AES_KEY_128_TO_255_NUM_OF_ROWS	(16U)
#define XNVM_EFUSE_USER_KEY0_0_TO_63_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_USER_KEY0_64_TO_191_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_USER_KEY0_192_TO_255_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_USER_KEY1_0_TO_63_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_USER_KEY1_64_TO_127_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_USER_KEY1_128_TO_255_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_PPK_HASH_NUM_OF_ROWS			(32U)
#define XNVM_EFUSE_METAHEADER_IV_NUM_OF_ROWS		(3U)
#define XNVM_EFUSE_BLACK_IV_NUM_OF_ROWS			(12U)
#define XNVM_EFUSE_PLM_IV_NUM_OF_ROWS			(12U)
#define XNVM_EFUSE_DATA_PARTITION_IV_NUM_OF_ROWS	(12U)
#define XNVM_EFUSE_GLITCH_DET_CONFIG_NUM_OF_ROWS	(4U)
#define XNVM_EFUSE_BOOT_MODE_NUM_OF_ROWS		(2U)
#define XNVM_EFUSE_DME_MODE_NUM_OF_ROWS			(1U)
#define XNVM_EFUSE_DME_FIPS_NUM_OF_ROWS			(1U)
#define XNVM_EFUSE_PAGE_0_PUF_SYN_DATA_NUM_OF_ROWS	(64U)
#define XNVM_EFUSE_PAGE_1_PUF_SYN_DATA_NUM_OF_ROWS      (63U)
#define XNVM_EFUSE_PUF_CHASH_NUM_OF_ROWS		(1U)
#define XNVM_EFUSE_PUF_AUX_NUM_OF_ROWS			(1U)
#define XNVM_EFUSE_PUF_RO_SWAP_NUM_OF_ROWS		(1U)
#define XNVM_EFUSE_DEC_ONLY_NUM_OF_ROWS			(4U)
#define XNVM_EFUSE_MISC_CTRL_NUM_OF_ROWS		(4U)
#define XNVM_EFUSE_SEC_CTRL_NUM_OF_ROWS			(4U)
#define XNVM_EFUSE_SECURITY_MISC1_NUM_OF_ROWS		(4U)
#define XNVM_EFUSE_BOOT_ENV_CTRL_NUM_OF_ROWS		(4U)
#define XNVM_EFUSE_DICE_UDS_0_TO_63_NUM_OF_ROWS		(8U)
#define XNVM_EFUSE_DICE_UDS_64_TO_191_NUM_OF_ROWS	(8U)
#define XNVM_EFUSE_DICE_UDS_192_TO_255_NUM_OF_ROWS	(2U)
#define XNVM_EFUSE_DICE_UDS_256_TO_383_NUM_OF_ROWS	(16U)
#define XNVM_EFUSE_DME_USER_KEY_NUM_OF_ROWS		(48U)
#define XNVM_EFUSE_CRC_NUM_OF_ROWS			(4U)
#define XNVM_EFUSE_CRC_SALT_NUM_OF_ROWS			(1U)

#define XNVM_EFUSE_CACHE_METAHEADER_IV_RANGE_OFFSET	(0x00000180U)
#define XNVM_EFUSE_CACHE_BLACK_IV_OFFSET		(0x000001D0U)
#define XNVM_EFUSE_CACHE_PLM_IV_RANGE_OFFSET		(0x000001DCU)
#define XNVM_EFUSE_CACHE_DATA_PARTITION_IV_OFFSET	(0x000001E8U)
#define XNVM_EFUSE_CACHE_SECURITY_CTRL_OFFSET		(0x000000ACU)
#define XNVM_EFUSE_CACHE_PPK0_HASH_OFFSET		(0x00000100U)
#define XNVM_EFUSE_CACHE_PPK1_HASH_OFFSET		(0x00000120U)
#define XNVM_EFUSE_CACHE_PPK2_HASH_OFFSET		(0x00000140U)
#define XNVM_EFUSE_CACHE_MISC_CTRL_CACHE_OFFSET		(0x000000A0U)
#define XNVM_EFUSE_CACHE_SEC_MISC_1_OFFSET		(0x000000E8U)
#define XNVM_EFUSE_CACHE_DME_FIPS_OFFSET		(0x00000234U)
#define XNVM_EFUSE_CACHE_IP_DISABLE_OFFSET		(0x00000018U)
#define XNVM_EFUSE_CACHE_PUF_ECC_PUF_CTRL_OFFSET	(0x000000A4U)
#define XNVM_EFUSE_CACHE_PUF_CHASH_OFFSET		(0x000000A8U)
#define XNVM_EFUSE_CACHE_ANLG_TRIM_3_OFFSET		(0x00000010U)
#define XNVM_EFUSE_CACHE_SECURITY_MISC_0_OFFSET		(0x000000E4U)
#define XNVM_EFUSE_CACHE_BOOT_ENV_CTRL_OFFSET		(0x00000094U)
#define XNVM_EFUSE_CACHE_PUF_SYN_DATA_OFFSET		(0x00000300U)
#define XNVM_EFUSE_CACHE_CRC_OFFSET			(0x0000023CU)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_OFFSET		(0x00000000U)
#define XNVM_EFUSE_CACHE_ME_ID_CODE_OFFSET		(0x000000FCU)

#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_REG_INIT_DIS_1_0_MASK	(0xc0000000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_BOOT_ENV_WR_LK_MASK	(0x10000000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_SEC_LOCK_DBG_DIS_MASK	(0x00600000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_SEC_DEBUG_DIS_MASK	(0x00180000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_PUF_DIS_MASK		(0x00040000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_PUF_TEST2_DIS_MASK	(0x00020000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_PUF_SYN_LK_MASK	(0x00010000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_USR_KEY_1_WR_LK_MASK	(0x00008000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_USR_KEY_1_CRC_LK_MASK	(0x00004000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_USR_KEY_0_WR_LK_MASK	(0x00002000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_USR_KEY_0_CRC_LK_MASK	(0x00001000U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_AES_WR_LK_MASK	(0x00000800U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_AES_CRC_LK_1_0_MASK	(0x00000600U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_PPK2_WR_LK_MASK	(0x00000100U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_PPK1_WR_LK_MASK	(0x00000080U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_PPK0_WR_LK_MASK	(0x00000040U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_JTAG_DIS_MASK		(0x00000004U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_JTAG_ERROUT_DIS_MASK	(0x00000002U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_AES_DIS_MASK		(0x00000001U)
#define XNVM_EFUSE_CACHE_SECURITY_CONTROL_UDS_WR_LK_MASK	(0x00000010U)

#define XNVM_EFUSE_CTRL_STATUS_UDS_DICE_CRC_PASS_MASK    	(0x00002000U)
#define XNVM_EFUSE_CTRL_STATUS_UDS_DICE_CRC_DONE_MASK    	(0x00001000U)
#define XNVM_EFUSE_CTRL_UDS_DICE_CRC_OFFSET            		(0x00000070U)

#define XNVM_EFUSE_CACHE_DME_FIPS_DME_MODE_MASK			(0x0000000FU)
#define XNVM_EFUSE_GLITCH_CONFIG_DATA_MASK			(0x7FFFFFFFU)

#define XNVM_EFUSE_CACHE_TBITS0_SVD_ANCHOR_3_MASK    		(0x08000000U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_ANCHOR_2_MASK    		(0x04000000U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_CRC_PROT_MASK    		(0x00300000U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_PUF_CHASH_PROT_MASK    	(0x00080000U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_SEC_MISC1_PROT_MASK    	(0x00040000U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_ANCHOR_1_MASK		(0x00000002U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_ANCHOR_0_MASK		(0x00000001U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_SEC_CTRL_PROT_MASK 		(0x02000004U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_SEC_MISC0_PROT_MASK 	(0x01000008U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_PPK_HASH_PROT_MASK		(0x00810000U)
#define XNVM_EFUSE_CACHE_TBITS0_SVD_META_HEADER_EXPORT_PROT_MASK	(0x00420000U)

#define XNVM_EFUSE_CACHE_ME_ID_CODE_CRC_SALT_MASK		(0xFF000000U)

#ifdef __cplusplus
}
#endif

#endif	/* XNVM_EFUSE_HW_H */
