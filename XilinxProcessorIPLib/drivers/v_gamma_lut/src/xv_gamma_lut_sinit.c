// ==============================================================
// Copyright (c) 1986 - 2022 Xilinx Inc. All rights reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// SPDX-License-Identifier: MIT
// ==============================================================

#ifndef __linux__

#include "xstatus.h"
#ifndef SDT
#include "xparameters.h"
#endif
#include "xv_gamma_lut.h"

#ifndef XPAR_XV_GAMMA_LUT_NUM_INSTANCES
#define XPAR_XV_GAMMA_LUT_NUM_INSTANCES   0
#endif

extern XV_gamma_lut_Config XV_gamma_lut_ConfigTable[];

#ifndef SDT
XV_gamma_lut_Config *XV_gamma_lut_LookupConfig(u16 DeviceId) {
	XV_gamma_lut_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XV_GAMMA_LUT_NUM_INSTANCES; Index++) {
		if (XV_gamma_lut_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XV_gamma_lut_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XV_gamma_lut_Initialize(XV_gamma_lut *InstancePtr, u16 DeviceId) {
	XV_gamma_lut_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XV_gamma_lut_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XV_gamma_lut_CfgInitialize(InstancePtr,
									  ConfigPtr,
									  ConfigPtr->BaseAddress);
}
#else
XV_gamma_lut_Config *XV_gamma_lut_LookupConfig(UINTPTR BaseAddress) {
	XV_gamma_lut_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; XV_gamma_lut_ConfigTable[Index].Name != NULL; Index++) {
		if ((XV_gamma_lut_ConfigTable[Index].BaseAddress == BaseAddress) ||
				!BaseAddress) {
			ConfigPtr = &XV_gamma_lut_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XV_gamma_lut_Initialize(XV_gamma_lut *InstancePtr, UINTPTR BaseAddress) {
	XV_gamma_lut_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XV_gamma_lut_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XV_gamma_lut_CfgInitialize(InstancePtr, ConfigPtr,
					  ConfigPtr->BaseAddress);
}
#endif
#endif
