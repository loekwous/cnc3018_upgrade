add_library(segger OBJECT)

target_include_directories(segger SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc ${CMAKE_CURRENT_SOURCE_DIR}/Config)

target_sources(segger PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_RTT_printf.c
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_RTT.c
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_SYSVIEW_Config_FreeRTOS.c
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_SYSVIEW_FreeRTOS.c
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_SYSVIEW.c
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_RTT_Syscalls_GCC.c
${CMAKE_CURRENT_SOURCE_DIR}/SEGGER_RTT_ASM_ARMv7M.S)