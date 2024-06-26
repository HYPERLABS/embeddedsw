/******************************************************************************
* Copyright (c) 2019 - 2021 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2022 - 2023 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/


#ifndef XPM_CPMDOMAIN_H_
#define XPM_CPMDOMAIN_H_

#include "xpm_bisr.h"
#include "xpm_powerdomain.h"
#include "xpm_regs.h"
#include "xpm_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * CPM domain node class.
 */
typedef struct XPm_CpmDomain {
	XPm_PowerDomain Domain; /**< Power domain node base class */
	u32 CpmSlcrBaseAddr; /**< CPM SLCR Base Address */
	u32 CpmSlcrSecureBaseAddr; /**< CPM SLCR Secure base address */
	u32 CpmPcsrBaseAddr; /**< CPM PCSR Base address */
	u32 CpmCrCpmBaseAddr; /**< CPM CRCPM Base address */
	SAVE_REGION()
} XPm_CpmDomain;

/************************** Function Prototypes ******************************/
XStatus XPmCpmDomain_Init(XPm_CpmDomain *CpmDomain, u32 Id, u32 BaseAddress,
			  XPm_Power *Parent, const u32 *OtherBaseAddresses,
			  u32 OtherBaseAddressesCnt);
#ifdef __cplusplus
}
#endif
/** @} */
#endif /* XPM_CPMDOMAIN_H_ */
