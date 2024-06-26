/******************************************************************************
* Copyright (c) 2023 - 2024 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/******************************************************************************
*
*
* riscv_invalidate_icache_range (unsigned int cacheaddr, unsigned int len)
*
*    Invalidate an instruction cache range and corresponding data cache range:
*    - Increment by line length in bytes
*
*    Parameters:
*       'cacheaddr' - address in the cache where invalidation begins
*	'len    '   - length (in bytes) worth of cache to be invalidated
*
*******************************************************************************/

#include "xpseudo_asm.h"
#include "xparameters.h"

.global	riscv_invalidate_icache_range
.section .text
.align 2
.type riscv_invalidate_icache_range, @function

#ifndef XPAR_MICROBLAZE_RISCV_ICACHE_LINE_LEN
#define XPAR_MICROBLAZE_RISCV_ICACHE_LINE_LEN     4
#endif

#define INCREMENT 4 * XPAR_MICROBLAZE_RISCV_ICACHE_LINE_LEN

riscv_invalidate_icache_range:
#if XPAR_MICROBLAZE_RISCV_USE_ICACHE==1
        beqz    a0, L_done            /* Skip loop if size is zero */
        add	t0, a0, a1            /* Compute end address */
        andi    t0, t0, -(INCREMENT)  /* Align end down to cache line */
        andi    a0, a0, -(INCREMENT)  /* Align start down to cache line */
L_start:
        cbo.flush (a0)                /* Flush the cache block (line) */
				      /* Flush is used since the instruction also affects the dcache */
        addi    a0, a0, (INCREMENT)   /* Increment the address by line length in bytes */
        bltu    a0, t0, L_start       /* Are we at the end? */
#endif
L_done:
	ret
.end riscv_invalidate_icache_range
