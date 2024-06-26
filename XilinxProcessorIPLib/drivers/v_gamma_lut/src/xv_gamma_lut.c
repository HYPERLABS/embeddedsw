// ==============================================================
// Copyright (c) 1986 - 2022 Xilinx Inc. All rights reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// SPDX-License-Identifier: MIT
// ==============================================================

/***************************** Include Files *********************************/
#include "xv_gamma_lut.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XV_gamma_lut_CfgInitialize(XV_gamma_lut *InstancePtr,
                               XV_gamma_lut_Config *ConfigPtr,
                               UINTPTR EffectiveAddr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);
    Xil_AssertNonvoid(EffectiveAddr != (UINTPTR)0x0);

    /* Setup the instance */
    InstancePtr->Config = *ConfigPtr;
    InstancePtr->Config.BaseAddress = EffectiveAddr;

    /* Set the flag to indicate the driver is ready */
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XV_gamma_lut_Start(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL) & 0x80;
    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XV_gamma_lut_IsDone(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XV_gamma_lut_IsIdle(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XV_gamma_lut_IsReady(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XV_gamma_lut_EnableAutoRestart(XV_gamma_lut *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL, 0x80);
}

void XV_gamma_lut_DisableAutoRestart(XV_gamma_lut *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_AP_CTRL, 0);
}

void XV_gamma_lut_Set_HwReg_width(XV_gamma_lut *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_HWREG_WIDTH_DATA, Data);
}

u32 XV_gamma_lut_Get_HwReg_width(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_HWREG_WIDTH_DATA);
    return Data;
}

void XV_gamma_lut_Set_HwReg_height(XV_gamma_lut *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_HWREG_HEIGHT_DATA, Data);
}

u32 XV_gamma_lut_Get_HwReg_height(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_HWREG_HEIGHT_DATA);
    return Data;
}

void XV_gamma_lut_Set_HwReg_video_format(XV_gamma_lut *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_HWREG_VIDEO_FORMAT_DATA, Data);
}

u32 XV_gamma_lut_Get_HwReg_video_format(XV_gamma_lut *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_HWREG_VIDEO_FORMAT_DATA);
    return Data;
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_0_BaseAddress(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_0_HighAddress(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_HIGH);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_0_TotalBytes(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + 1);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_0_BitWidth(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_GAMMA_LUT_CTRL_WIDTH_HWREG_GAMMA_LUT_0;
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_0_Depth(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_GAMMA_LUT_CTRL_DEPTH_HWREG_GAMMA_LUT_0;
}

u32 XV_gamma_lut_Write_HwReg_gamma_lut_0_Words(XV_gamma_lut *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XV_gamma_lut_Read_HwReg_gamma_lut_0_Words(XV_gamma_lut *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + (offset + i)*4);
    }
    return length;
}

u32 XV_gamma_lut_Write_HwReg_gamma_lut_0_Bytes(XV_gamma_lut *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XV_gamma_lut_Read_HwReg_gamma_lut_0_Bytes(XV_gamma_lut *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_0_BASE + offset + i);
    }
    return length;
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_1_BaseAddress(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_1_HighAddress(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_HIGH);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_1_TotalBytes(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + 1);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_1_BitWidth(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_GAMMA_LUT_CTRL_WIDTH_HWREG_GAMMA_LUT_1;
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_1_Depth(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_GAMMA_LUT_CTRL_DEPTH_HWREG_GAMMA_LUT_1;
}

u32 XV_gamma_lut_Write_HwReg_gamma_lut_1_Words(XV_gamma_lut *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XV_gamma_lut_Read_HwReg_gamma_lut_1_Words(XV_gamma_lut *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + (offset + i)*4);
    }
    return length;
}

u32 XV_gamma_lut_Write_HwReg_gamma_lut_1_Bytes(XV_gamma_lut *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XV_gamma_lut_Read_HwReg_gamma_lut_1_Bytes(XV_gamma_lut *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_1_BASE + offset + i);
    }
    return length;
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_2_BaseAddress(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_2_HighAddress(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_HIGH);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_2_TotalBytes(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + 1);
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_2_BitWidth(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_GAMMA_LUT_CTRL_WIDTH_HWREG_GAMMA_LUT_2;
}

u32 XV_gamma_lut_Get_HwReg_gamma_lut_2_Depth(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_GAMMA_LUT_CTRL_DEPTH_HWREG_GAMMA_LUT_2;
}

u32 XV_gamma_lut_Write_HwReg_gamma_lut_2_Words(XV_gamma_lut *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XV_gamma_lut_Read_HwReg_gamma_lut_2_Words(XV_gamma_lut *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + (offset + i)*4);
    }
    return length;
}

u32 XV_gamma_lut_Write_HwReg_gamma_lut_2_Bytes(XV_gamma_lut *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XV_gamma_lut_Read_HwReg_gamma_lut_2_Bytes(XV_gamma_lut *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_HIGH - XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Config.BaseAddress + XV_GAMMA_LUT_CTRL_ADDR_HWREG_GAMMA_LUT_2_BASE + offset + i);
    }
    return length;
}

void XV_gamma_lut_InterruptGlobalEnable(XV_gamma_lut *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_GIE, 1);
}

void XV_gamma_lut_InterruptGlobalDisable(XV_gamma_lut *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_GIE, 0);
}

void XV_gamma_lut_InterruptEnable(XV_gamma_lut *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_IER);
    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_IER, Register | Mask);
}

void XV_gamma_lut_InterruptDisable(XV_gamma_lut *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_IER);
    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_IER, Register & (~Mask));
}

void XV_gamma_lut_InterruptClear(XV_gamma_lut *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XV_gamma_lut_WriteReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_ISR, Mask);
}

u32 XV_gamma_lut_InterruptGetEnabled(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_IER);
}

u32 XV_gamma_lut_InterruptGetStatus(XV_gamma_lut *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XV_gamma_lut_ReadReg(InstancePtr->Config.BaseAddress, XV_GAMMA_LUT_CTRL_ADDR_ISR);
}
