cmake_minimum_required(VERSION 3.13)

if(NOT BUILD_FIRMWARE)
    return()
endif()

project(uart_fw C ASM)

# Ensure toolchain and linker
if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Set -DCMAKE_TOOLCHAIN_FILE=toolchains/arm-gcc.cmake")
endif()

SET(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/common/linker/stm32h5/stm32h563xx.ld")

# Define executable
add_executable(uart_echo
    ${CMAKE_SOURCE_DIR}/projects/uart/main.c
    ${CMAKE_SOURCE_DIR}/common/drivers/uart/uart_core.c
    ${CMAKE_SOURCE_DIR}/common/drivers/uart/ringbuf.c
    ${CMAKE_SOURCE_DIR}/projects/uart/src/uart_echo.c
    ${CMAKE_SOURCE_DIR}/common/platform/baremetal/stm32h5/uart_hw.c
    ${CMAKE_SOURCE_DIR}/common/platform/baremetal/stm32h5/startup_stm32h5.s
)

# Use altenate propery setting here on executable format (just for an example thereof)
# This could be specified in one go when adding the executable
set_target_properties(uart_echo PROPERTIES SUFFIX ".elf")

# Set include paths
target_include_directories(uart_echo PRIVATE
    ${CMAKE_SOURCE_DIR}/common/include
    ${CMAKE_SOURCE_DIR}/common/drivers/uart
    ${CMAKE_SOURCE_DIR}/common/platform/baremetal/stm32h5
)

# Specs and linker script per target, avoiding globals
target_link_options(uart_echo PRIVATE "-Wl,-Map,$<TARGET_FILE_DIR:uart_echo>/$<TARGET_FILE_BASE_NAME:uart_echo>.map")
target_link_options(uart_echo PRIVATE "-T${LINKER_SCRIPT}")
target_link_options(uart_echo PRIVATE "-Wl,--gc-sections")
target_link_options(uart_echo PRIVATE
    "-specs=nano.specs"
    "-specs=nosys.specs"
)

# HEX/BIN post-build
add_custom_command(TARGET uart_echo POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:uart_echo> $<TARGET_FILE_DIR:uart_echo>/$<TARGET_FILE_BASE_NAME:uart_echo>.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:uart_echo> $<TARGET_FILE_DIR:uart_echo>/$<TARGET_FILE_BASE_NAME:uart_echo>.bin
    COMMENT "Generating HEX & BIN"
)

