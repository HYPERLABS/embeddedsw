#
# Copyright (C) 2015 - 2019 Xilinx, Inc.
#
# This file is part of the FreeRTOS port.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# http://www.FreeRTOS.org
# http://aws.amazon.com/freertos
#
# 1 tab == 4 spaces!
#

# -----------------------------------------------
# Return latest "ACTIVE" standalone BSP version
# -----------------------------------------------
proc get_standalone_version {} {
	set standalone_path [glob -directory "../" standalone_v*]
	return [lindex [split $standalone_path /] end]
}

proc FreeRTOS_drc {os_handle} {

	global env

	set sw_proc_handle [hsi::get_sw_processor]
	set hw_proc_handle [hsi::get_cells -hier [common::get_property HW_INSTANCE $sw_proc_handle] ]
	set proctype [common::get_property IP_NAME $hw_proc_handle]

	if { $proctype == "microblaze" } {
		mb_drc_checks $sw_proc_handle $hw_proc_handle $os_handle
	}
}

proc Check_ttc_ip {instance_name} {
	set cortexa53cpu [hsi::get_cells -hier -filter "IP_NAME==psu_cortexa53"]
	if {[llength $cortexa53cpu] > 0} {
		set ttc_ips [get_cell -hier -filter {IP_NAME== "psu_ttc"}]
	} else {
		set ttc_ips [get_cell -hier -filter {IP_NAME== "ps7_ttc"}]
	}

	if { [llength $ttc_ips] != 0 } {
		foreach ttc_ip $ttc_ips {
			if {[string compare -nocase $ttc_ip $instance_name] == 0} {
				set isintr [::hsm::utils::is_ip_interrupting_current_proc $ttc_ip]
				if {$isintr == 1} {
					return
				} else {
					error "FreeRTOS requires timer with interrupt enabled. $ttc_ip is not connected to interrupt controller."
				}
			}
		}
	}
	error "FreeRTOS requires valid ticker timer. The HW platform doesn't have a specified ticker timer $instance_name."
}
proc generate {os_handle} {

	set standalone_version [get_standalone_version]
	set have_tick_timer 0
	set sw_proc_handle [hsi::get_sw_processor]
	set hw_proc_handle [hsi::get_cells -hier [common::get_property HW_INSTANCE $sw_proc_handle] ]
	set proctype [common::get_property IP_NAME $hw_proc_handle]
	set need_config_file "false"
	set enable_sw_profile [common::get_property CONFIG.enable_sw_intrusive_profiling $os_handle]
	set is_versal [hsi::get_cells -hier -filter "IP_NAME==psu_cortexa72"]
	# proctype should be "microblaze", ps7_cortexa9, psu_cortexr5 or psu_cortexa53


	switch $proctype {

		"psu_cortexr5"  {
				puts "In start copy psu_cortexr5"
				file copy -force "./src/Makefile_psu_cortexr5" "./src/Makefile"
				file copy -force "./src/Makefile" "./src/Makefile_dep"
				if { $enable_sw_profile == "true" } {
					error "ERROR: Profiling is not supported for R5"
				}
				set need_config_file "true"

                                set pss_ref_clk_mhz [common::get_property CONFIG.C_PSS_REF_CLK_FREQ $hw_proc_handle]
	                        if { $pss_ref_clk_mhz == "" } {
		                        puts "WARNING: CONFIG.C_PSS_REF_CLK_FREQ not found. Using default value for XPAR_PSU_PSS_REF_CLK_FREQ_HZ."
		                        set pss_ref_clk_mhz 33333000
	                        }
				set file_handle [::hsi::utils::open_include_file "xparameters.h"]
                                puts $file_handle " /* Definition for PSS REF CLK FREQUENCY */"
		                puts $file_handle [format %s%.0f%s "#define XPAR_PSU_PSS_REF_CLK_FREQ_HZ " [expr $pss_ref_clk_mhz]  "U"]
	                        puts $file_handle ""
				puts $file_handle "#include \"xparameters_ps.h\""
				puts $file_handle ""

				#Tweak DDR addresses for versal, to be removed once proper addresses added by PCW
				if {[llength $is_versal] > 0} {
					puts $file_handle "#define XPAR_PSU_R5_DDR_0_S_AXI_BASEADDR 0x00100000"
					puts $file_handle "#define XPAR_PSU_R5_DDR_0_S_AXI_HIGHADDR 0x7FFFFFFF"
					puts $file_handle ""
				}
				close $file_handle
			}
		"psu_cortexa53"  {
				set procdrv [hsi::get_sw_processor]
			        set compiler [get_property CONFIG.compiler $procdrv]
				if {[string compare -nocase $compiler "arm-none-eabi-gcc"] == 0} {
					error "ERROR: FreeRTOS is not supported for 32bit A53"
				}
				puts "In start copy psu_cortexa53"
				file copy -force "./src/Makefile_psu_cortexa53" "./src/Makefile"
				file copy -force "./src/Makefile" "./src/Makefile_dep"
				if { $enable_sw_profile == "true" } {
					error "ERROR: Profiling is not supported for A53"
				}
				set need_config_file "true"

                                set pss_ref_clk_mhz [common::get_property CONFIG.C_PSS_REF_CLK_FREQ $hw_proc_handle]
	                        if { $pss_ref_clk_mhz == "" } {
	                                puts "WARNING: CONFIG.C_PSS_REF_CLK_FREQ not found. Using default value for XPAR_PSU_PSS_REF_CLK_FREQ_HZ."
	                                set pss_ref_clk_mhz 33333000
                                }
				set file_handle [::hsi::utils::open_include_file "xparameters.h"]
                                puts $file_handle " /* Definition for PSS REF CLK FREQUENCY */"
	                        puts $file_handle [format %s%.0f%s "#define XPAR_PSU_PSS_REF_CLK_FREQ_HZ " [expr $pss_ref_clk_mhz]  "U"]
                                puts $file_handle ""
				puts $file_handle "#include \"xparameters_ps.h\""
				puts $file_handle ""
				close $file_handle
			}
		"ps7_cortexa9"  {
				puts "In start copy ps7_cortexa9"
				file copy -force "./src/Makefile_ps7_cortexa9" "./src/Makefile"
				file copy -force "./src/Makefile" "./src/Makefile_dep"
				set need_config_file "true"

				set file_handle [::hsi::utils::open_include_file "xparameters.h"]
				puts $file_handle "#include \"xparameters_ps.h\""
				puts $file_handle ""
				close $file_handle
			}

		"microblaze"  {
				puts "In start copy microblaze"
				file copy -force "./src/Makefile_microblaze" "./src/Makefile"
				file copy -force "./src/Makefile" "./src/Makefile_dep"

				set need_config_file "true"
			}

		"default" {
			puts "processor type $proctype not supported\n"
		}
	}


	# Copy core kernel files to the main src directory
	file copy -force [file join src Source tasks.c] ./src
	file copy -force [file join src Source queue.c] ./src
	file copy -force [file join src Source list.c] ./src
	file copy -force [file join src Source timers.c] ./src
	file copy -force [file join src Source event_groups.c] ./src
	file copy -force [file join src Source portable MemMang heap_4.c] ./src

	if { $proctype == "psu_cortexr5" } {
		file copy -force [file join src Source portable GCC ARM_CR5 port.c] ./src
		file copy -force [file join src Source portable GCC ARM_CR5 portASM.S] ./src
		file copy -force [file join src Source portable GCC ARM_CR5 port_asm_vectors.S] ./src
		file copy -force [file join src Source portable GCC ARM_CR5 portmacro.h] ./src
		file copy -force [file join src Source portable GCC ARM_CR5 portZynqUltrascale.c] ./src
	}
	if { $proctype == "psu_cortexa53" } {
		file copy -force [file join src Source portable GCC ARM_CA53 port.c] ./src
		file copy -force [file join src Source portable GCC ARM_CA53 portASM.S] ./src
		file copy -force [file join src Source portable GCC ARM_CA53 port_asm_vectors.S] ./src
		file copy -force [file join src Source portable GCC ARM_CA53 portmacro.h] ./src
		file copy -force [file join src Source portable GCC ARM_CA53 portZynqUltrascale.c] ./src
	}

	if { $proctype == "ps7_cortexa9" } {
		file copy -force [file join src Source portable GCC ARM_CA9 port.c] ./src
		file copy -force [file join src Source portable GCC ARM_CA9 portASM.S] ./src
		file copy -force [file join src Source portable GCC ARM_CA9 port_asm_vectors.S] ./src
		file copy -force [file join src Source portable GCC ARM_CA9 portmacro.h] ./src
		file copy -force [file join src Source portable GCC ARM_CA9 portZynq7000.c] ./src
	}
	# Create bspconfig file
	set bspcfg_fn [file join ".." "${standalone_version}" "src" "common" "bspconfig.h"]
	file delete $bspcfg_fn
	set bspcfg_fh [open $bspcfg_fn w]
	xprint_generated_header $bspcfg_fh "Configurations for Standalone BSP"

	puts $bspcfg_fh "\#ifndef __BSPCONFIG_H_"
	puts $bspcfg_fh "\#define __BSPCONFIG_H_"
	puts $bspcfg_fh ""

	if { $proctype == "microblaze" } {
		file copy -force [file join src Source portable GCC MicroBlazeV9 port.c] ./src
		file copy -force [file join src Source portable GCC MicroBlazeV9 port_exceptions.c] ./src
		file copy -force [file join src Source portable GCC MicroBlazeV9 portasm.S] ./src
		file copy -force [file join src Source portable GCC MicroBlazeV9 portmacro.h] ./src
		file copy -force [file join src Source portable GCC MicroBlazeV9 portmicroblaze.c] ./src

		# Create config file for microblaze interrupt handling
		if {[string compare -nocase $need_config_file "true"] == 0} {
			xhandle_mb_interrupts
		}

		# Create config files for Microblaze exception handling
		if { [mb_has_exceptions $hw_proc_handle] } {
			xcreate_mb_exc_config_file
		}


		if { [mb_has_pvr $hw_proc_handle] } {

			set pvr [get_property CONFIG.C_PVR $hw_proc_handle]

			switch $pvr {
				"0" {
					puts $bspcfg_fh "#define MICROBLAZE_PVR_NONE"
				}
				"1" {
					puts $bspcfg_fh "#define MICROBLAZE_PVR_BASIC"
				}
				"2" {
					puts $bspcfg_fh "#define MICROBLAZE_PVR_FULL"
				}
				"default" {
					puts $bspcfg_fh "#define MICROBLAZE_PVR_NONE"
				}
			}
		}

	}

	puts $bspcfg_fh "/*"
	puts $bspcfg_fh " * Definition to indicate that current BSP is a FreeRTOS BSP which can be used to"
	puts $bspcfg_fh " * distinguish between standalone BSP and FreeRTOS BSP."
	puts $bspcfg_fh " */"
	puts $bspcfg_fh "#define FREERTOS_BSP"
	if { $proctype == "psu_cortexa53" } {
		if {[string compare -nocase $compiler "arm-none-eabi-gcc"] != 0} {
			puts $bspcfg_fh "#define EL3 1"
			puts $bspcfg_fh "#define EL1_NONSECURE 0"
			puts $bspcfg_fh "#define HYP_GUEST 0"
		}
	}
	puts $bspcfg_fh ""
	puts $bspcfg_fh "\#endif /*end of __BSPCONFIG_H_*/"
	close $bspcfg_fh
	
	set file_handle [::hsi::utils::open_include_file "xparameters.h"]
	
	puts $file_handle "/* Platform specific definitions */" 
	if { $proctype == "psu_cortexa53" || $proctype == "psu_cortexr5"} {
		puts $file_handle "#define PLATFORM_ZYNQMP"
	}
	if { $proctype == "ps7_cortexa9"} {
		puts $file_handle "#define PLATFORM_ZYNQ"
	}
	if { $proctype == "microblaze"} {
		puts $file_handle "#define PLATFORM_MB"
	}
	
	if {[llength $is_versal] > 0} {
		puts $file_handle "#ifndef versal"
		puts $file_handle "#define versal"
		puts $file_handle "#endif"
		puts $file_handle ""
	}
	puts $file_handle " "
	puts $file_handle "/******************************************************************/"
	close $file_handle

	set headers [glob -join ./src/Source/include *.\[h\]]
	foreach header $headers {
		file copy -force $header src
	}

	file delete -force [file join src Source]

	# Handle stdin
	set stdin [common::get_property CONFIG.stdin $os_handle]
	if { $stdin == "" || $stdin == "none" } {
                handle_stdin_parameter $os_handle
	} else {
		::hsi::utils::handle_stdin $os_handle
	}
	# Handle stdout
	set stdout [common::get_property CONFIG.stdout $os_handle]
	if { $stdout == "" || $stdout == "none" } {
		handle_stdout_parameter $os_handle
	} else {
		::hsi::utils::handle_stdout $os_handle
	}

	file copy -force "./src/outbyte.c" "../${standalone_version}/src/"
	file copy -force "./src/inbyte.c" "../${standalone_version}/src/"
	xcreate_cmake_toolchain_file $os_handle $is_versal $standalone_version
	set file_handle [::hsi::utils::open_include_file "xparameters.h"]
	puts $file_handle "\n/******************************************************************/\n"
	set val [common::get_property CONFIG.enable_stm_event_trace $os_handle]
	if { $val == "true" } {
		if { $proctype == "psu_cortexr5" || $proctype == "psu_cortexa53" } {
			variable stm_trace_header_data
			puts $file_handle "/* Enable event trace through STM */"
			puts $file_handle "#define FREERTOS_ENABLE_TRACE"
			set val [common::get_property CONFIG.enable_timer_tick_trace $os_handle]
			if { $val == "true" } {
				puts $file_handle "#define FREERTOS_ENABLE_TIMER_TICK_TRACE"
			}
			set val [common::get_property CONFIG.stm_channel $os_handle]
			if { ![string is double -strict $val] || [catch {expr $val >> 8}] || $val < 0 || $val > 65535 } {
				error "Invalid STM channel $val. Please set a value between 0 - 65535"
			}
			puts $file_handle "#define FREERTOS_STM_CHAN $val"
			if { $proctype == "psu_cortexa53" && [common::get_property CONFIG.exec_mode $sw_proc_handle] == "aarch64" } {
				puts $file_handle "#define EXEC_MODE64"
			} else {
				puts $file_handle "#define EXEC_MODE32"
			}
			puts $file_handle "\n/******************************************************************/\n"
		} else {
			puts "WARNING: STM event trace is not supported for $proctype"
		}
	}
	close $file_handle

	############################################################################
	## Add constants common to all architectures to the configuration file.
	############################################################################

	set config_file [xopen_new_include_file "./src/FreeRTOSConfig.h" "FreeRTOS Configuration parameters"]
	puts $config_file "\#include \"xparameters.h\" \n"

	set val [common::get_property CONFIG.use_preemption $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_PREEMPTION" "0"
	} else {
		xput_define $config_file "configUSE_PREEMPTION" "1"
	}

	set val [common::get_property CONFIG.use_mutexes $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_MUTEXES" "0"
	} else {
		xput_define $config_file "configUSE_MUTEXES" "1"
	}

        set val [common::get_property CONFIG.use_getmutex_holder $os_handle]
        if {$val == "false"} {
                xput_define $config_file "INCLUDE_xSemaphoreGetMutexHolder" "0"
        } else {
                xput_define $config_file "INCLUDE_xSemaphoreGetMutexHolder" "1"
        }

	set val [common::get_property CONFIG.use_recursive_mutexes $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_RECURSIVE_MUTEXES" "0"
	} else {
		xput_define $config_file "configUSE_RECURSIVE_MUTEXES" "1"
	}

	set val [common::get_property CONFIG.use_counting_semaphores $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_COUNTING_SEMAPHORES" "0"
	} else {
		xput_define $config_file "configUSE_COUNTING_SEMAPHORES" "1"
	}

	set val [common::get_property CONFIG.use_timers $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_TIMERS" "0"
	} else {
		xput_define $config_file "configUSE_TIMERS" "1"
	}

	set val [common::get_property CONFIG.use_idle_hook $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_IDLE_HOOK"	"0"
	} else {
		xput_define $config_file "configUSE_IDLE_HOOK"	"1"
	}

	set val [common::get_property CONFIG.use_tick_hook $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_TICK_HOOK"	"0"
	} else {
		xput_define $config_file "configUSE_TICK_HOOK"	"1"
	}

        set val [common::get_property CONFIG.use_daemon_task_startup_hook $os_handle]
        if {$val == "false"} {
                xput_define $config_file "configUSE_DAEMON_TASK_STARTUP_HOOK"  "0"
        } else {
                xput_define $config_file "configUSE_DAEMON_TASK_STARTUP_HOOK"  "1"
        }

	set val [common::get_property CONFIG.use_malloc_failed_hook $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_MALLOC_FAILED_HOOK"	"0"
	} else {
		xput_define $config_file "configUSE_MALLOC_FAILED_HOOK"	"1"
	}

	set val [common::get_property CONFIG.use_trace_facility $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_TRACE_FACILITY" "0"
	} else {
		xput_define $config_file "configUSE_TRACE_FACILITY" "1"
	}

        set val [common::get_property CONFIG.use_newlib_reent $os_handle]
        if {$val == "false"} {
                xput_define $config_file "configUSE_NEWLIB_REENTRANT" "0"
        } else {
                xput_define $config_file "configUSE_NEWLIB_REENTRANT" "1"
        }

	xput_define $config_file "configUSE_16_BIT_TICKS"		   "0"
	xput_define $config_file "configUSE_APPLICATION_TASK_TAG"   "0"
	xput_define $config_file "configUSE_CO_ROUTINES"			"0"

	set tick_rate [common::get_property CONFIG.tick_rate $os_handle]
	xput_define $config_file "configTICK_RATE_HZ"	 "($tick_rate)"

	set max_priorities [common::get_property CONFIG.max_priorities $os_handle]
	xput_define $config_file "configMAX_PRIORITIES"   "($max_priorities)"
	xput_define $config_file "configMAX_CO_ROUTINE_PRIORITIES" "2"

	set min_stack [common::get_property CONFIG.minimal_stack_size $os_handle]
	set min_stack [expr [expr $min_stack + 3] & 0xFFFFFFFC]
	xput_define $config_file "configMINIMAL_STACK_SIZE" "( ( unsigned short ) $min_stack)"

	set total_heap_size [common::get_property CONFIG.total_heap_size $os_handle]
	xput_define $config_file "configTOTAL_HEAP_SIZE"  "( ( size_t ) ( $total_heap_size ) )"

	set max_task_name_len [common::get_property CONFIG.max_task_name_len $os_handle]
	xput_define $config_file "configMAX_TASK_NAME_LEN"  $max_task_name_len

	set val [common::get_property CONFIG.idle_yield $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configIDLE_SHOULD_YIELD"  "0"
	} else {
		xput_define $config_file "configIDLE_SHOULD_YIELD"  "1"
	}

        set val [common::get_property CONFIG.use_timeslicing $os_handle]
        if {$val == "false"} {
                xput_define $config_file "configUSE_TIME_SLICING"  "0"
        } else {
                xput_define $config_file "configUSE_TIME_SLICING"  "1"
        }

	set val [common::get_property CONFIG.timer_task_priority $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configTIMER_TASK_PRIORITY"  "0"
	} else {
		xput_define $config_file "configTIMER_TASK_PRIORITY"  $val
	}

	set val [common::get_property CONFIG.timer_command_queue_length $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configTIMER_QUEUE_LENGTH"  "0"
	} else {
		xput_define $config_file "configTIMER_QUEUE_LENGTH"  $val
	}

	set val [common::get_property CONFIG.timer_task_stack_depth $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configTIMER_TASK_STACK_DEPTH"  "0"
	} else {
		xput_define $config_file "configTIMER_TASK_STACK_DEPTH"  "($val * 2)"
	}

	set val [get_property CONFIG.use_freertos_asserts $os_handle]
	if {$val == "true"} {
		puts $config_file "#define configASSERT( x ) if( ( x ) == 0 ) vApplicationAssert( __FILE__, __LINE__ )\n"
	}

	set val [common::get_property CONFIG.use_queue_sets $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_QUEUE_SETS"  "0"
	} else {
		xput_define $config_file "configUSE_QUEUE_SETS"  "1"
	}

        set val [common::get_property CONFIG.use_task_notifications $os_handle]
        if {$val == "false"} {
                xput_define $config_file "configUSE_TASK_NOTIFICATIONS"  "0"
        } else {
                xput_define $config_file "configUSE_TASK_NOTIFICATIONS"  "1"
        }

	set val [common::get_property CONFIG.check_for_stack_overflow $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configCHECK_FOR_STACK_OVERFLOW"  "0"
	} else {
		if { $val > 2 } {
			error "ERROR: check_for_stack_overflow must be between 0 and 2"
		} else {
			xput_define $config_file "configCHECK_FOR_STACK_OVERFLOW"  $val
		}
	}

        set val [common::get_property CONFIG.use_task_fpu_support $os_handle]
        if { $val < 1 || $val > 2 } {
                error "ERROR: use_task_fpu_support must be 1 or 2"
        } else {
                xput_define $config_file "configUSE_TASK_FPU_SUPPORT"  $val

        }

	set val [common::get_property CONFIG.queue_registry_size $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configQUEUE_REGISTRY_SIZE"  "0"
	} else {
		xput_define $config_file "configQUEUE_REGISTRY_SIZE"  $val
	}


	set val [common::get_property CONFIG.use_stats_formatting_functions  $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configUSE_STATS_FORMATTING_FUNCTIONS"  "0"
	} else {
		xput_define $config_file "configUSE_STATS_FORMATTING_FUNCTIONS"  "1"
	}

	set val [common::get_property CONFIG.num_thread_local_storage_pointers $os_handle]
	if {$val == "false"} {
		xput_define $config_file "configNUM_THREAD_LOCAL_STORAGE_POINTERS"  "0"
	} else {
		xput_define $config_file "configNUM_THREAD_LOCAL_STORAGE_POINTERS"  $val
	}

	puts $config_file "#define configUSE_TICKLESS_IDLE	0"
	puts $config_file "#define configTASK_RETURN_ADDRESS    NULL"
	puts $config_file "#define INCLUDE_vTaskPrioritySet             1"
	puts $config_file "#define INCLUDE_uxTaskPriorityGet            1"
	puts $config_file "#define INCLUDE_vTaskDelete                  1"
	puts $config_file "#define INCLUDE_vTaskCleanUpResources        1"
	puts $config_file "#define INCLUDE_vTaskSuspend                 1"
	puts $config_file "#define INCLUDE_vTaskDelayUntil              1"
	puts $config_file "#define INCLUDE_vTaskDelay                   1"
	puts $config_file "#define INCLUDE_eTaskGetState                1"
	puts $config_file "#define INCLUDE_xTimerPendFunctionCall       1"
	puts $config_file "#define INCLUDE_pcTaskGetTaskName            1"

	############################################################################
	## Add constants specific to the psu_cortexr5
	############################################################################

	if { $proctype == "psu_cortexr5" } {

		set val [common::get_property CONFIG.PSU_TTC0_Select $os_handle]
		if {$val == "true"} {
			set have_tick_timer 1
			Check_ttc_ip "psu_ttc_0"
			set val1 [common::get_property CONFIG.PSU_TTC0_Select_Cntr $os_handle]
			if {$val1 == "0"} {
				xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_0_DEVICE_ID"
				xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_0_BASEADDR"
				xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_0_INTR"
			} else {
				if {$val1 == "1"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_1_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_1_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_1_INTR"
				} else {
					if {$val1 == "2"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_2_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_2_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_2_INTR"
					} else {
						error "ERROR: invalid timer selected" "mdt_error"
					}
				}
			}
		}

		set val [common::get_property CONFIG.PSU_TTC1_Select $os_handle]
		if {$val == "true"} {
		if {$have_tick_timer == 1} {
				error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
			} else {
				set have_tick_timer 1
				Check_ttc_ip "psu_ttc_1"
				set val1 [common::get_property CONFIG.PSU_TTC1_Select_Cntr $os_handle]
				if {$val1 == "0"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_3_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_3_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_3_INTR"
				} else {
					if {$val1 == "1"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_4_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_4_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_4_INTR"
					} else {
						if {$val1 == "2"} {
							xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_5_DEVICE_ID"
							xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_5_BASEADDR"
							xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_5_INTR"
						} else {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
		}

		set val [common::get_property CONFIG.PSU_TTC2_Select $os_handle]
		if {$val == "true"} {
			if {$have_tick_timer == 1} {
				error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
			} else {
				set have_tick_timer 1
				Check_ttc_ip "psu_ttc_2"
				set val1 [common::get_property CONFIG.PSU_TTC2_Select_Cntr $os_handle]
				if {$val1 == "0"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_6_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_6_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_6_INTR"
				} else {
					if {$val1 == "1"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_7_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_7_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_7_INTR"
					} else {
						if {$val1 == "2"} {
							xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_8_DEVICE_ID"
							xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_8_BASEADDR"
							xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_8_INTR"
						} else {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
		}

		set val [common::get_property CONFIG.PSU_TTC3_Select $os_handle]
		if {$val == "true"} {
			if {$have_tick_timer == 1} {
				error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
			} else {
				set have_tick_timer 1
				Check_ttc_ip "psu_ttc_3"
				set val1 [common::get_property CONFIG.PSU_TTC3_Select_Cntr $os_handle]
				if {$val1 == "0"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_9_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_9_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_9_INTR"
				} else {
					if {$val1 == "1"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_10_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_10_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_10_INTR"
					} else {
						if {$val1 == "2"} {
							xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_11_DEVICE_ID"
							xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_11_BASEADDR"
							xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_11_INTR"
						} else {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
		}

		if {$have_tick_timer == 0} {
			error "ERROR: No tick timer selected " "mdt_error"
		}
		xput_define $config_file "configUNIQUE_INTERRUPT_PRIORITIES"			   "32"
		xput_define $config_file "configINTERRUPT_CONTROLLER_DEVICE_ID"			"XPAR_SCUGIC_SINGLE_DEVICE_ID"
		xput_define $config_file "configINTERRUPT_CONTROLLER_BASE_ADDRESS"		 "XPAR_SCUGIC_0_DIST_BASEADDR"
		xput_define $config_file "configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET" "0x1000"

		# Function prototypes cannot be in the common code as some compilers or
		# ports require pre-processor guards to ensure they are not visible from
		# assembly files.
		puts $config_file "void vApplicationAssert( const char *pcFile, uint32_t ulLine );"
		puts $config_file "void FreeRTOS_SetupTickInterrupt( void );"
		puts $config_file "#define configSETUP_TICK_INTERRUPT() FreeRTOS_SetupTickInterrupt()\n"
		puts $config_file "void FreeRTOS_ClearTickInterrupt( void );"
		puts $config_file "#define configCLEAR_TICK_INTERRUPT()	FreeRTOS_ClearTickInterrupt()\n"
		puts $config_file "#define configGENERATE_RUN_TIME_STATS 0\n"
		puts $config_file "#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()\n"
		puts $config_file "#define portGET_RUN_TIME_COUNTER_VALUE()\n"
		puts $config_file "#define configCOMMAND_INT_MAX_OUTPUT_SIZE 2096\n"
		puts $config_file "#define recmuCONTROLLING_TASK_PRIORITY ( configMAX_PRIORITIES - 2 )\n"

		set max_api_call_interrupt_priority [common::get_property CONFIG.max_api_call_interrupt_priority $os_handle]
		xput_define $config_file "configMAX_API_CALL_INTERRUPT_PRIORITY"   "($max_api_call_interrupt_priority)"

		set val [common::get_property CONFIG.use_port_optimized_task_selection $os_handle]
		if {$val == "false"} {
			xput_define $config_file "configUSE_PORT_OPTIMISED_TASK_SELECTION"  "0"
		} else {
			xput_define $config_file "configUSE_PORT_OPTIMISED_TASK_SELECTION"  "1"
		}
	}
	# end of if $proctype == "psu_cortexr5"

	############################################################################
	## Add constants specific to the psu_cortexa53
	############################################################################

	if { $proctype == "psu_cortexa53" } {

		set val [common::get_property CONFIG.PSU_TTC0_Select $os_handle]
		if {$val == "true"} {
			set have_tick_timer 1
			Check_ttc_ip "psu_ttc_0"
			set val1 [common::get_property CONFIG.PSU_TTC0_Select_Cntr $os_handle]
			if {$val1 == "0"} {
				xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_0_DEVICE_ID"
				xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_0_BASEADDR"
				xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_0_INTR"
			} else {
				if {$val1 == "1"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_1_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_1_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_1_INTR"
				} else {
					if {$val1 == "2"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_2_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_2_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_2_INTR"
					} else {
						error "ERROR: invalid timer selected" "mdt_error"
					}
				}
			}
		}

		set val [common::get_property CONFIG.PSU_TTC1_Select $os_handle]
		if {$val == "true"} {
		if {$have_tick_timer == 1} {
				error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
			} else {
				set have_tick_timer 1
				Check_ttc_ip "psu_ttc_1"
				set val1 [common::get_property CONFIG.PSU_TTC1_Select_Cntr $os_handle]
				if {$val1 == "0"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_3_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_3_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_3_INTR"
				} else {
					if {$val1 == "1"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_4_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_4_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_4_INTR"
					} else {
						if {$val1 == "2"} {
							xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_5_DEVICE_ID"
							xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_5_BASEADDR"
							xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_5_INTR"
						} else {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
		}

		set val [common::get_property CONFIG.PSU_TTC2_Select $os_handle]
		if {$val == "true"} {
			if {$have_tick_timer == 1} {
				error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
			} else {
				set have_tick_timer 1
				Check_ttc_ip "psu_ttc_2"
				set val1 [common::get_property CONFIG.PSU_TTC2_Select_Cntr $os_handle]
				if {$val1 == "0"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_6_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_6_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_6_INTR"
				} else {
					if {$val1 == "1"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_7_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_7_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_7_INTR"
					} else {
						if {$val1 == "2"} {
							xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_8_DEVICE_ID"
							xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_8_BASEADDR"
							xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_8_INTR"
						} else {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
		}

		set val [common::get_property CONFIG.PSU_TTC3_Select $os_handle]
		if {$val == "true"} {
			if {$have_tick_timer == 1} {
				error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
			} else {
				set have_tick_timer 1
				Check_ttc_ip "psu_ttc_3"
				set val1 [common::get_property CONFIG.PSU_TTC3_Select_Cntr $os_handle]
				if {$val1 == "0"} {
					xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_9_DEVICE_ID"
					xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_9_BASEADDR"
					xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_9_INTR"
				} else {
					if {$val1 == "1"} {
						xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_10_DEVICE_ID"
						xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_10_BASEADDR"
						xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_10_INTR"
					} else {
						if {$val1 == "2"} {
							xput_define $config_file "configTIMER_ID" "XPAR_XTTCPS_11_DEVICE_ID"
							xput_define $config_file "configTIMER_BASEADDR" "XPAR_XTTCPS_11_BASEADDR"
							xput_define $config_file "configTIMER_INTERRUPT_ID" "XPAR_XTTCPS_11_INTR"
						} else {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
		}

		if {$have_tick_timer == 0} {
			error "ERROR: No tick timer selected " "mdt_error"
		}
		xput_define $config_file "configUNIQUE_INTERRUPT_PRIORITIES"			   "32"
		xput_define $config_file "configINTERRUPT_CONTROLLER_DEVICE_ID"			"XPAR_SCUGIC_SINGLE_DEVICE_ID"
		xput_define $config_file "configINTERRUPT_CONTROLLER_BASE_ADDRESS"		 "XPAR_SCUGIC_0_DIST_BASEADDR"
		xput_define $config_file "configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET" 	"0x10000"

		# Function prototypes cannot be in the common code as some compilers or
		# ports require pre-processor guards to ensure they are not visible from
		# assembly files.
		puts $config_file "void vApplicationAssert( const char *pcFile, uint32_t ulLine );"
		puts $config_file "void FreeRTOS_SetupTickInterrupt( void );"
		puts $config_file "#define configSETUP_TICK_INTERRUPT() FreeRTOS_SetupTickInterrupt()\n"
		puts $config_file "void FreeRTOS_ClearTickInterrupt( void );"
		puts $config_file "#define configCLEAR_TICK_INTERRUPT()	FreeRTOS_ClearTickInterrupt()\n"
		puts $config_file "#define configGENERATE_RUN_TIME_STATS 0\n"
		puts $config_file "#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()\n"
		puts $config_file "#define portGET_RUN_TIME_COUNTER_VALUE()\n"
		puts $config_file "#define configCOMMAND_INT_MAX_OUTPUT_SIZE 2096\n"
		puts $config_file "#define recmuCONTROLLING_TASK_PRIORITY ( configMAX_PRIORITIES - 2 )\n"
		puts $config_file "#define fabs( x ) __builtin_fabs( x )\n"
		set max_api_call_interrupt_priority [common::get_property CONFIG.max_api_call_interrupt_priority $os_handle]
		xput_define $config_file "configMAX_API_CALL_INTERRUPT_PRIORITY"   "($max_api_call_interrupt_priority)"

		set val [common::get_property CONFIG.use_port_optimized_task_selection $os_handle]
		if {$val == "false"} {
			xput_define $config_file "configUSE_PORT_OPTIMISED_TASK_SELECTION"  "0"
		} else {
			xput_define $config_file "configUSE_PORT_OPTIMISED_TASK_SELECTION"  "1"
		}
	}
	# end of if $proctype == "psu_cortexa53"

	############################################################################
	## Add constants specific to the microblaze
	############################################################################

	if { $proctype == "microblaze" } {
		set ttc_ips [get_cell -hier -filter {IP_NAME== "psu_ttc"}]
		if { [llength $ttc_ips] != 0 } {
			foreach ttc_ip $ttc_ips {
			if { $ttc_ip == "psu_ttc_0"} {
				set val [common::get_property CONFIG.PSU_TTC0_Select $os_handle]
				if {$val == "true"} {
					set have_tick_timer 1
					Check_ttc_ip "psu_ttc_0"
					set val1 [common::get_property CONFIG.PSU_TTC0_Select_Cntr $os_handle]
					set intr_pin_name [hsi::get_pins -of_objects [hsi::get_cells -hier $ttc_ip] [format "ps_pl_irq_ttc0_%d" $val1] ]
					set intcname [::hsi::utils::get_connected_intr_cntrl $ttc_ip  $intr_pin_name]
					puts $config_file "#define configTIMER_ID [format "XPAR_XTTCPS_%d_DEVICE_ID" $val1 ]"
					puts $config_file "#define configTIMER_BASEADDR [format "XPAR_XTTCPS_%d_BASEADDR" $val1 ]"
					puts $config_file "#define configTIMER_INTERRUPT_ID [string toupper [format XPAR_${intcname}_${ttc_ip}_${intr_pin_name}_INTR ] ]"
					if { $val1 >=3 } {
						error "ERROR: invalid timer selected" "mdt_error"
					}
				}
			}
			if { $ttc_ip == "psu_ttc_1"} {
				set val [common::get_property CONFIG.PSU_TTC1_Select $os_handle]
				if {$val == "true"} {
					if {$have_tick_timer == 1} {
						error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
					} else {
						set have_tick_timer 1
						Check_ttc_ip "psu_ttc_1"
						set val1 [common::get_property CONFIG.PSU_TTC1_Select_Cntr $os_handle]
						set intr_pin_name [hsi::get_pins -of_objects [hsi::get_cells -hier $ttc_ip] [format "ps_pl_irq_ttc1_%d" $val1] ]
						set intcname [::hsi::utils::get_connected_intr_cntrl $ttc_ip  $intr_pin_name]
						puts $config_file "#define configTIMER_ID [format "XPAR_XTTCPS_%d_DEVICE_ID" [ expr $val1+3 ] ]"
						puts $config_file "#define configTIMER_BASEADDR [format "XPAR_XTTCPS_%d_BASEADDR" [ expr $val1+3 ] ]"
						puts $config_file "#define configTIMER_INTERRUPT_ID [string toupper [format XPAR_${intcname}_${ttc_ip}_${intr_pin_name}_INTR ] ]"
						if { $val1 >=3 } {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
			if { $ttc_ip == "psu_ttc_2"} {
				set val [common::get_property CONFIG.PSU_TTC2_Select $os_handle]
				if {$val == "true"} {
					if {$have_tick_timer == 1} {
						error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
					} else {
						set have_tick_timer 1
						Check_ttc_ip "psu_ttc_2"
						set val1 [common::get_property CONFIG.PSU_TTC2_Select_Cntr $os_handle]
						set intr_pin_name [hsi::get_pins -of_objects [hsi::get_cells -hier $ttc_ip] [format "ps_pl_irq_ttc2_%d" $val1] ]
						set intcname [::hsi::utils::get_connected_intr_cntrl $ttc_ip  $intr_pin_name]
						puts $config_file "#define configTIMER_ID [format "XPAR_XTTCPS_%d_DEVICE_ID" [ expr $val1+6 ] ]"
						puts $config_file "#define configTIMER_BASEADDR [format "XPAR_XTTCPS_%d_BASEADDR" [ expr $val1+6 ] ]"
						puts $config_file "#define configTIMER_INTERRUPT_ID [string toupper [format XPAR_${intcname}_${ttc_ip}_${intr_pin_name}_INTR ] ]"
						if { $val1 >=3 } {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
			if { $ttc_ip == "psu_ttc_3"} {
				set val [common::get_property CONFIG.PSU_TTC3_Select $os_handle]
				if {$val == "true"} {
					if {$have_tick_timer == 1} {
						error "ERROR: Cannot select multiple timers for tick generation " "mdt_error"
					} else {
						set have_tick_timer 1
						Check_ttc_ip "psu_ttc_3"
						set val1 [common::get_property CONFIG.PSU_TTC3_Select_Cntr $os_handle]
						set intr_pin_name [hsi::get_pins -of_objects [hsi::get_cells -hier $ttc_ip] [format "ps_pl_irq_ttc2_%d" $val1] ]
						set intcname [::hsi::utils::get_connected_intr_cntrl $ttc_ip  $intr_pin_name]
						puts $config_file "#define configTIMER_ID [format "XPAR_XTTCPS_%d_DEVICE_ID" [ expr $val1+9 ] ]"
						puts $config_file "#define configTIMER_BASEADDR [format "XPAR_XTTCPS_%d_BASEADDR" [ expr $val1+9 ] ]"
						puts $config_file "#define configTIMER_INTERRUPT_ID [string toupper [format XPAR_${intcname}_${ttc_ip}_${intr_pin_name}_INTR ] ]"
						if { $val1 >=3 } {
							error "ERROR: invalid timer selected " "mdt_error"
						}
					}
				}
			}
			}
			if {$have_tick_timer == 0} {
				error "ERROR: No tick timer selected " "mdt_error"
			}
		}		
	}

	############################################################################
	## Add constants specific to the ps7_cortexa9
	############################################################################
	if { $proctype == "ps7_cortexa9" } {
		set max_api_call_interrupt_priority [common::get_property CONFIG.max_api_call_interrupt_priority $os_handle]
		xput_define $config_file "configMAX_API_CALL_INTERRUPT_PRIORITY"   "($max_api_call_interrupt_priority)"

		set val [common::get_property CONFIG.use_port_optimized_task_selection $os_handle]
		if {$val == "false"} {
			xput_define $config_file "configUSE_PORT_OPTIMISED_TASK_SELECTION"  "0"
		} else {
			xput_define $config_file "configUSE_PORT_OPTIMISED_TASK_SELECTION"  "1"
		}

		puts $config_file "#define configINTERRUPT_CONTROLLER_BASE_ADDRESS         ( XPAR_PS7_SCUGIC_0_DIST_BASEADDR )"
		puts $config_file "#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ( -0xf00 )"
		puts $config_file "#define configUNIQUE_INTERRUPT_PRIORITIES                32"

		# Function prototypes cannot be in the common code as some compilers or
		# ports require pre-processor guards to ensure they are not visible from
		# assembly files.
		puts $config_file "void vApplicationAssert( const char *pcFile, uint32_t ulLine );"
		puts $config_file "void FreeRTOS_SetupTickInterrupt( void );"
		puts $config_file "#define configSETUP_TICK_INTERRUPT() FreeRTOS_SetupTickInterrupt()\n"
		puts $config_file "void FreeRTOS_ClearTickInterrupt( void );"
		puts $config_file "#define configCLEAR_TICK_INTERRUPT()	FreeRTOS_ClearTickInterrupt()\n"
	}
	# end of if $proctype == "ps7_cortexa9"



	############################################################################
	## Add constants specific to the microblaze
	############################################################################
	if { $proctype == "microblaze" } {
		# Interrupt controller setting assumes only one is in use.
		puts $config_file "#define configINTERRUPT_CONTROLLER_TO_USE XPAR_INTC_SINGLE_DEVICE_ID"
		puts $config_file "#define configINSTALL_EXCEPTION_HANDLERS 1"

		# Avoid non #define statements getting included in assembly files.
		puts $config_file "#ifndef __ASSEMBLER__"
		puts $config_file "void vApplicationAssert( const char *pcFile, uint32_t ulLine );"
		puts $config_file "#endif"
	}
	# end of if $proctype == "microblaze"


	# include header file with STM trace macros
	puts $config_file "#ifdef FREERTOS_ENABLE_TRACE"
	puts $config_file "#include \"FreeRTOSSTMTrace.h\""
	puts $config_file "#endif /* FREERTOS_ENABLE_TRACE */\n"
	# complete the header protectors
	puts $config_file "\#endif"
	close $config_file
}


proc xcreate_cmake_toolchain_file {os_handle is_versal standalone_version} {
	# Get the processor
	set proc_instance [hsi::get_sw_processor]
	set hw_processor [common::get_property HW_INSTANCE $proc_instance]
	set proc_type [common::get_property IP_NAME [hsi::get_cells -hier $hw_processor]];
	set os [common::get_property NAME [hsi::get_os]]
	set compiler_str [common::get_property CONFIG.compiler -object ${proc_instance}]
	set compiler_l [split ${compiler_str}]
	set compiler [lindex ${compiler_l} 0]
	set crosscompile [string map {gcc ""} "${compiler}"]
	set c_flags [common::get_property CONFIG.compiler_flags -object ${proc_instance}]
	set extra_flags [common::get_property CONFIG.extra_compiler_flags -object ${proc_instance}]
	set linclude [file normalize "../.."]
	set extra_flags "${extra_flags} -I${linclude}/include"
	set standalone_dir "../${standalone_version}"
	set standalone_cmake_path "../src/run_cmake"

	# Generate cmake toolchain file and run cmake to generate make file
	set toolchain_cmake "toolchain"
	set toolchain_file "../src/${toolchain_cmake}.cmake"
	set bdir "${standalone_dir}/build_xilstandalone"
	if { [catch {file mkdir "${bdir}"} msg] } {
		error "Failed to create xilstandalone build directory."
	}
	set workdir [pwd]
	cd "${bdir}"
	set cmake_cmd ${standalone_cmake_path}
	set os_platform_type "$::tcl_platform(platform)"
	set fd [open "${toolchain_file}" w]

	if { "${proc_type}" == "psu_cortexr5" } {
		puts $fd "set (CMAKE_SYSTEM_PROCESSOR \"cortexr5\" CACHE STRING \"\")"
		if { [llength $is_versal] > 0} {
			puts $fd "set (MACHINE \"versal\")"
		} else {
			puts $fd "set (MACHINE \"ZynqMP\")"
		}
	} elseif { "${proc_type}" == "psu_cortexa53"} {
		if { "${compiler}" == "arm-none-eabi-gcc" } {
			puts $fd "set (CMAKE_SYSTEM_PROCESSOR \"cortexa53-32\" CACHE STRING \"\")"
			puts $fd "set (MACHINE \"ZynqMP\")"
		} else {
			puts $fd "set (CMAKE_SYSTEM_PROCESSOR \"cortexa53\" CACHE STRING \"\")"
			puts $fd "set (MACHINE \"ZynqMP\")"
		}
	} elseif { "${proc_type}" == "psu_cortexa72"} {
			puts $fd "set (CMAKE_SYSTEM_PROCESSOR \"cortexa72\" CACHE STRING \"\")"
			puts $fd "set (MACHINE \"versal\")"
	} elseif { "${proc_type}" == "ps7_cortexa9" } {
		puts $fd "set (CMAKE_SYSTEM_PROCESSOR \"cortexa9\" CACHE STRING \"\")"
		puts $fd "set (MACHINE \"Zynq\")"
	} elseif { "${proc_type}" == "microblaze" || "${proc_type}" == "psu_pmc" || "${proc_type}" == "psu_psm"} {
		puts $fd "set (CMAKE_SYSTEM_PROCESSOR \"microblaze\" CACHE STRING \"\")"
		puts $fd "set (MACHINE \"microblaze_generic\")"
		set c_flags "${c_flags}  -mlittle-endian"
	}
	puts $fd "set (CROSS_PREFIX \"${crosscompile}\" CACHE STRING \"\")"
	puts $fd "set (CMAKE_C_FLAGS \"${c_flags} ${extra_flags}\" CACHE STRING \"\")"
	puts $fd "set (CMAKE_ASM_FLAGS \"${c_flags} ${extra_flags}\" CACHE STRING \"\")"
	if { [string match "freertos*" "${os}"] > 0 } {
		puts $fd "set (CMAKE_SYSTEM_NAME \"FreeRTOS\" CACHE STRING \"\")"
	} else {
		puts $fd "set (CMAKE_SYSTEM_NAME \"Generic\" CACHE STRING \"\")"
	}
	puts $fd "include (CMakeForceCompiler)"
	puts $fd "CMAKE_FORCE_C_COMPILER (\"\$\{CROSS_PREFIX\}gcc\" GNU)"
	puts $fd "CMAKE_FORCE_CXX_COMPILER (\"\$\{CROSS_PREFIX\}g++\" GNU)"

	puts $fd "set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE STRING \"\")"
	puts $fd "set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER CACHE STRING \"\")"
	puts $fd "set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER CACHE STRING \"\")"
	close $fd

	
	#copy toolchain file to libsrc directory of BSP
	file copy -force ${toolchain_file} ../../

	set cmake_opt "-DCMAKE_TOOLCHAIN_FILE=${toolchain_file}"
	append cmake_opt " -DCMAKE_INSTALL_PREFIX=/"
	append cmake_opt " -DCMAKE_VERBOSE_MAKEFILE=on"
	append cmake_opt " -DWITH_DEFAULT_LOGGER=off"

	if { [string match -nocase "windows*" "${os_platform_type}"] == 0 } {
		# Linux
		file attributes ${cmake_cmd} -permissions ugo+rx
		if { [catch {exec ${cmake_cmd} "../src"  ${cmake_opt}} msg] } {
			error "Failed to generate cmake files.${msg}"
		} else {
			puts "${msg}"
		}
	} else {
		# Windows
		# Note: windows tcl exec does not do well when trying to provide ${cmake_opt}
		#       for now hardcoding the values directly on the command line.
		if { [catch {exec ${cmake_cmd} "../src" -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../src/toolchain.cmake -DCMAKE_INSTALL_PREFIX=/ -DCMAKE_VERBOSE_MAKEFILE=on -DWITH_DEFAULT_LOGGER=off} msg] } {
			error "Failed to generate cmake files.${msg}"
		} else {
			puts "${msg}"
		}
	}
	
	cd "${workdir}"

}

proc xopen_new_include_file { filename description } {
	set inc_file [open $filename w]
	xprint_generated_header $inc_file $description
	set newfname [string map {. _} [lindex [split $filename {\/}] end]]
	puts $inc_file "\#ifndef _[string toupper $newfname]"
	puts $inc_file "\#define _[string toupper $newfname]\n\n"
	return $inc_file
}

proc xput_define { config_file parameter param_value } {
	puts $config_file "#define $parameter $param_value\n"
}

proc xhandle_mb_interrupts {} {

	set default_interrupt_handler "XNullHandler"
	set default_arg "XNULL"

	set source_interrupt_handler $default_interrupt_handler
	set source_handler_arg $default_arg

	# Handle the interrupt pin
	set sw_proc_handle [get_sw_processor]
	set periph [get_cells -hier $sw_proc_handle]
	set source_ports [xget_interrupt_sources $periph]
	if {[llength $source_ports] > 1} {
		error "Too many interrupting ports on the MicroBlaze.  Should only find 1" "" "error"
		return
	}

	if {[llength $source_ports] == 1} {
		set source_port [lindex $source_ports 0]
		if {[llength $source_port] != 0} {
			set source_port_name [get_property NAME $source_port]
			set source_periph [get_cells -of_objects $source_port]
			set source_name [get_property NAME $source_periph]
			set source_driver [get_drivers $source_name]

			if {[string compare -nocase $source_driver ""] != 0} {
				set int_array [get_arrays -of_objects $source_driver]
				if {[llength $int_array] != 0} {
					set size [get_property PROPERTY.size $int_array]
					for {set i 0 } { $i < $size } { incr $i } {
						set int_port [lindex [get_property PARAM.int_port $int_array] $i]
						if {[llength $int_port] != 0} {
							if {[string compare -nocase $int_port $source_port_name] == 0 } {
								set source_interrupt_handler [lindex [get_property PARAM.int_handler $int_array] $i ]
								set source_handler_arg [lindex [get_property PARAM.int_handler_arg $int_array] $i ]
								if {[string compare -nocase $source_handler_arg DEVICE_ID] == 0 } {
									set source_handler_arg [xget_name $source_periph "DEVICE_ID"]
								} else {
									if {[llength $source_periph] == 0} {
										set source_handler_arg $default_arg
									} else {
										set source_handler_arg [xget_name $source_periph "C_BASEADDR"]
									}
								}
								break
							}
						}
					}
				}
			}
		}
    }

    # Generate microblaze_interrupts_g.c file...
    xcreate_mb_intr_config_file $source_interrupt_handler $source_handler_arg
}

proc xcreate_mb_intr_config_file {handler arg} {

    set mb_table "MB_InterruptVectorTable"
    set standalone_version [get_standalone_version]

	set filename [file join ".." "${standalone_version}" "src" "microblaze_interrupts_g.c"]
    file delete $filename
    set config_file [open $filename w]

    xprint_generated_header $config_file "Interrupt Handler Table for MicroBlaze Processor"

    puts $config_file "#include \"microblaze_interrupts_i.h\""
    puts $config_file "#include \"xparameters.h\""
    puts $config_file "\n"
    puts $config_file [format "extern void %s (void *);" $handler]
    puts $config_file "\n/*"
    puts $config_file "* The interrupt handler table for microblaze processor"
    puts $config_file "*/\n"
    puts $config_file [format "%sEntry %s\[\] =" $mb_table $mb_table]
    puts $config_file "\{"
    puts -nonewline $config_file [format "\{\t%s" $handler]
    puts -nonewline $config_file [format ",\n\t(void*) %s\}" $arg]
    puts -nonewline $config_file "\n\};"
    puts $config_file "\n"
    close $config_file
}

# --------------------------------------
# Return true if this MB has
# exception handling support
# --------------------------------------
proc mb_has_exceptions { hw_proc_handle } {

    # Check if the following parameters exist on this MicroBlaze's MPD
    set ee [get_property CONFIG.C_UNALIGNED_EXCEPTIONS $hw_proc_handle]
    if { $ee != "" } {
        return true
    }

    set ee [get_property CONFIG.C_ILL_OPCODE_EXCEPTION $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    set ee [get_property CONFIG.C_IOPB_BUS_EXCEPTION  $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    set ee [get_property CONFIG.C_DOPB_BUS_EXCEPTION  $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    set ee [get_property CONFIG.C_DIV_BY_ZERO_EXCEPTION $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    set ee [get_property CONFIG.C_DIV_ZERO_EXCEPTION $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    set ee [get_property CONFIG.C_FPU_EXCEPTION   $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    set ee [get_property CONFIG.C_USE_MMU    $hw_proc_handle]
    if { $ee != "" } {
        return true
    }
    return false
}

# -------------------------------------------
# Tcl procedure xcreate_mb_exc_config file
# -------------------------------------------
proc xcreate_mb_exc_config_file { } {

    set hfilename [file join "src" "microblaze_exceptions_g.h"]
    file delete $hfilename
    set hconfig_file [open $hfilename w]

    xprint_generated_header $hconfig_file "Exception Handling Header for MicroBlaze Processor"

    puts $hconfig_file "\n"

    set sw_proc_handle [get_sw_processor]
    set hw_proc_handle [get_cells -hier [get_property HW_INSTANCE $sw_proc_handle] ]
    set proctype [get_property IP_NAME $hw_proc_handle]
    set procver [get_ip_version $hw_proc_handle]

    if { ![mb_has_exceptions $hw_proc_handle]} { ;# NO exceptions are enabled
        close $hconfig_file              ;# Do not generate any info in either the header or the C file
        return
    }

    puts $hconfig_file "\#define MICROBLAZE_EXCEPTIONS_ENABLED 1"
    if { [mb_can_handle_exceptions_in_delay_slots $procver] } {
        puts $hconfig_file "#define MICROBLAZE_CAN_HANDLE_EXCEPTIONS_IN_DELAY_SLOTS"
    }

    close $hconfig_file
}

# --------------------------------------
# Return true if MB ver 'procver' has
# support for handling exceptions in
# delay slots
# --------------------------------------
proc mb_can_handle_exceptions_in_delay_slots { procver } {

    if { [string compare -nocase $procver "5.00.a"] >= 0 } {
        return true
    } else {
        return false
    }
}

# --------------------------------------
# Return true if this MB has PVR support
# --------------------------------------
proc mb_has_pvr { hw_proc_handle } {

    # Check if the following parameters exist on this MicroBlaze's MPD
    set pvr [get_property CONFIG.C_PVR $hw_proc_handle]
    if { $pvr != "" } {
        return true
    }

    return false
}

# --------------------------------------
# Microblaze config checks
# --------------------------------------
proc mb_drc_checks { sw_proc_handle hw_proc_handle os_handle } {
	set compiler [common::get_property CONFIG.compiler $sw_proc_handle]

	# check for valid compiler
	if { [string first "mb-gcc" $compiler] == 0 && [string first "mb-g++" $compiler] == 0} {
		error "Wrong compiler requested. FreeRTOS can be compiled only with the GNU compiler for MicroBlaze." "" "mdt_error"
	}

	# check for valid stdio parameters
	set stdin  [common::get_property CONFIG.stdin  $os_handle]
	set stdout [common::get_property CONFIG.stdout $os_handle]
	if { $stdin == "none" || $stdout == "none" } {
		error "The STDIN/STDOUT parameters are not set. FreeRTOS requires stdin/stdout to be set." "" "mdt_error"
	}

	# check if the design has a intc
	set intr_port [hsi::get_pins -of_objects $hw_proc_handle Interrupt]
	set intr_flag 1
	if { [llength $intr_port] == 0 } {
		set intr_flag 0
	} else {
		set intr_net [hsi::get_nets -of_objects $intr_port]
		if  { [llength $intr_net] == 0 }  {
			set intr_flag 0
		}
	}

	if {$intr_flag == 0 } {
		error "CPU has no connection to Interrupt controller." "" "mdt_error"
	}

	# support only AXI/PLB
	set bus_name ""
	set interconnect [common::get_property CONFIG.C_INTERCONNECT $hw_proc_handle]
	puts [format "hw_proc_handle is %s" $hw_proc_handle]
	if { $interconnect == 2 } {
		set intf_pin [hsi::get_intf_pins -of_objects $hw_proc_handle "M_AXI_DP"]
		if { [llength $intf_pin] } {
			set bus_name [hsi::get_intf_nets -of_objects $intf_pin]
		}
	} else {
		error "FreeRTOS supports Microblaze with only a AXI interconnect" "" "mdt_error"
	}

	if { [llength $bus_name] == 0 } {
		error "Microblaze M_AXI_DP is not connected to slave peripherals"
	}

	# obtain handles to all the peripherals in the design
	set slave_ifs [hsi::get_intf_pins -of_objects $bus_name -filter "TYPE==SLAVE"]
	puts [format "slave_ifs %s bus_name %s" $slave_ifs $bus_name]
	set timer_count 0
	set timer_has_intr 0

	# check for a valid timer
        set axi_timer_ips [get_cell -hier -filter {IP_NAME== "axi_timer"}]
        if { [llength $axi_timer_ips] != 0 } {
             foreach axi_timer_ip $axi_timer_ips {
                 incr timer_count
		 # check if the axi_timer IP is interrupting current processor
                 set isintr [::hsm::utils::is_ip_interrupting_current_proc $axi_timer_ip]
                 if {$isintr == 1} {
                     set timer_has_intr 1
                 }
              }
         } else {
		    set ttc_ips [get_cell -hier -filter {IP_NAME== "psu_ttc"}]
			if { [llength $ttc_ips] != 0 } {
			     foreach ttc_ip $ttc_ips {
				     incr timer_count
			  # check if the axi_timer IP is interrupting current processor
			         set isintr [::hsm::utils::is_ip_interrupting_current_proc $ttc_ip]
			         if {$isintr == 1} {
                         set timer_has_intr 1
                     }
			     }
			 }
		 }

	if { $timer_count == 0 } {
		error "FreeRTOS for Microblaze requires an axi_timer or xps_timer. The HW platform doesn't have a valid timer." "" "mdt_error"
	}

	if { $timer_has_intr == 0 } {
		error "FreeRTOS for Microblaze requires interrupts enabled for a timer." "" "mdt_error"
	}


}

#
# Handle the stdout parameter of a processor
#
proc handle_stdout_parameter {drv_handle} {
   set stdout [common::get_property CONFIG.stdout $drv_handle]
   set sw_proc_handle [::hsi::get_sw_processor]
   set hw_proc_handle [::hsi::get_cells -hier [common::get_property hw_instance $sw_proc_handle]]
   set processor [common::get_property NAME $hw_proc_handle]

   if {[llength $stdout] == 1 && [string compare -nocase "none" $stdout] != 0} {

       set stdout_drv_handle [::hsi::get_drivers -filter "HW_INSTANCE==$stdout"]
       if {[llength $stdout_drv_handle] == 0} {
           error "No driver for stdout peripheral $stdout. Check the following reasons: \n
                  1. $stdout is not accessible from processor $processor.\n
                  2. No Driver block is defined for $stdout in MSS file." "" "hsi_error"
           return
       }

       set interface_handle [::hsi::get_sw_interfaces -of_objects $stdout_drv_handle -filter "NAME==stdout"]
       if {[llength $interface_handle] == 0} {
         error "No stdout interface available for driver for peripheral $stdout" "" "hsi_error"
       }
       set outbyte_name [common::get_property FUNCTION.outbyte $interface_handle]
       if {[llength $outbyte_name] == 0} {
         error "No outbyte function available for driver for peripheral $stdout" "" "hsi_error"
       }
       set header [common::get_property PROPERTY.header $interface_handle]
       if {[llength $header] == 0} {
         error "No header property available in stdout interface for driver for peripheral $stdout" "" "hsi_error"
       }
       set config_file [open "src/outbyte.c" w]
       puts $config_file "\#include \"xparameters.h\""
       puts $config_file [format "\#include \"%s\"\n" $header ]
       puts $config_file "\#ifdef __cplusplus"
       puts $config_file "extern \"C\" {"
       puts $config_file "\#endif"
       puts $config_file "void outbyte(char c); \n"
       puts $config_file "\#ifdef __cplusplus"
       puts $config_file "}"
       puts $config_file "\#endif \n"
       puts $config_file "void outbyte(char c) {"
       puts $config_file [format "\t %s(STDOUT_BASEADDRESS, c);" $outbyte_name]
       puts $config_file "}"
       close $config_file
       set config_file [::hsi::utils::open_include_file "xparameters.h"]
       set stdout_mem_range [::hsi::get_mem_ranges -of_objects $hw_proc_handle -filter "INSTANCE==$stdout && IS_DATA==1" ]
       if { [llength $stdout_mem_range] > 1 } {
           set stdout_mem_range [::hsi::get_mem_ranges -of_objects $hw_proc_handle -filter "INSTANCE==$stdout&& (BASE_NAME==C_BASEADDR||BASE_NAME==C_S_AXI_BASEADDR)"]
       }
       set base_name [common::get_property BASE_NAME $stdout_mem_range]
       set base_value [common::get_property BASE_VALUE $stdout_mem_range]
       puts $config_file "\#define STDOUT_BASEADDRESS [::hsi::utils::format_addr_string $base_value $base_name]"
       close $config_file
   } else {
            if { $stdout == "" || $stdout == "none" } {
                    #
                    # UART is not present in the system, add dummy implementatin for outbyte
                    #
                    set config_file [open "src/outbyte.c" w]
		    puts $config_file "\#include \"xparameters.h\""
		    puts $config_file "\#ifdef __cplusplus"
		    puts $config_file "extern \"C\" {"
		    puts $config_file "\#endif"
		    puts $config_file "void outbyte(char c); \n"
		    puts $config_file "\#ifdef __cplusplus"
		    puts $config_file "}"
		    puts $config_file "\#endif \n"
		    puts $config_file "void outbyte(char c) {"
		    puts $config_file "}"
                    close $config_file
            }
     }
}

#
# Handle the stdin parameter of a processor
#
proc handle_stdin_parameter {drv_handle} {

   set stdin [common::get_property CONFIG.stdin $drv_handle]
   set sw_proc_handle [::hsi::get_sw_processor]
   set hw_proc_handle [::hsi::get_cells -hier [common::get_property hw_instance $sw_proc_handle]]

   set processor [common::get_property hw_instance $sw_proc_handle]
   if {[llength $stdin] == 1 && [string compare -nocase "none" $stdin] != 0} {
       set stdin_drv_handle [::hsi::get_drivers -filter "HW_INSTANCE==$stdin"]
       if {[llength $stdin_drv_handle] == 0} {
           error "No driver for stdin peripheral $stdin. Check the following reasons: \n
                  1. $stdin is not accessible from processor $processor.\n
                  2. No Driver block is defined for $stdin in MSS file." "" "hsi_error"
           return
       }

       set interface_handle [::hsi::get_sw_interfaces -of_objects $stdin_drv_handle -filter "NAME==stdin"]
       if {[llength $interface_handle] == 0} {
           error "No stdin interface available for driver for peripheral $stdin" "" "hsi_error"
       }

       set inbyte_name [common::get_property FUNCTION.inbyte $interface_handle ]
       if {[llength $inbyte_name] == 0} {
         error "No inbyte function available for driver for peripheral $stdin" "" "hsi_error"
       }
       set header [common::get_property PROPERTY.header $interface_handle]
       if {[llength $header] == 0} {
         error "No header property available in stdin interface for driver for peripheral $stdin" "" "hsi_error"
       }
       set config_file [open "src/inbyte.c" w]
       puts $config_file "\#include \"xparameters.h\""
       puts $config_file [format "\#include \"%s\"\n" $header]
       puts $config_file "\#ifdef __cplusplus"
       puts $config_file "extern \"C\" {"
       puts $config_file "\#endif"
       puts $config_file "char inbyte(void);"
       puts $config_file "\#ifdef __cplusplus"
       puts $config_file "}"
       puts $config_file "\#endif \n"
       puts $config_file "char inbyte(void) {"
       puts $config_file [format "\t return %s(STDIN_BASEADDRESS);" $inbyte_name]
       puts $config_file "}"
       close $config_file
       set config_file [::hsi::utils::open_include_file "xparameters.h"]
       set stdin_mem_range [::hsi::get_mem_ranges -of_objects $hw_proc_handle -filter "INSTANCE==$stdin && IS_DATA==1"]
       if { [llength $stdin_mem_range] > 1 } {
           set stdin_mem_range [::hsi::get_mem_ranges -of_objects $hw_proc_handle -filter "INSTANCE==$stdin&& (BASE_NAME==C_BASEADDR||BASE_NAME==C_S_AXI_BASEADDR)"]
       }
       set base_name [common::get_property BASE_NAME $stdin_mem_range]
       set base_value [common::get_property BASE_VALUE $stdin_mem_range]
       puts $config_file "\#define STDIN_BASEADDRESS [::hsi::utils::format_addr_string $base_value $base_name]"
       close $config_file
   } else {
            if { $stdin == "" || $stdin == "none" } {
                    #
                    # UART is not present in the system, add dummy implementatin for inbyte
                    #
                    set config_file [open "src/inbyte.c" w]
                    puts $config_file "\#include \"xparameters.h\""
                    puts $config_file "\#ifdef __cplusplus"
                    puts $config_file "extern \"C\" {"
                    puts $config_file "\#endif"
                    puts $config_file "char inbyte(void);"
                    puts $config_file "\#ifdef __cplusplus"
                    puts $config_file "}"
                    puts $config_file "\#endif \n"
                    puts $config_file "char inbyte(void) {"
                    puts $config_file "}"
                    close $config_file
            }
     }
}
