cmake_minimum_required(VERSION 3.13)

if(NOT BUILD_FIRMWARE)
    return()
endif()

project(blinky_fw C ASM)

# Ensure toolchain and linker
if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Set -DCMAKE_TOOLCHAIN_FILE=toolchains/arm-gcc.cmake")
endif()

SET(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/common/linker/stm32h5/stm32h563xx.ld")

# Define executable
add_executable(blinky.elf
    ${CMAKE_SOURCE_DIR}/projects/blinky/main.c
    ${CMAKE_SOURCE_DIR}/projects/blinky/app/blinky.c
    ${CMAKE_SOURCE_DIR}/common/platform/baremetal/stm32h5/gpio.c
    ${CMAKE_SOURCE_DIR}/common/platform/baremetal/stm32h5/startup_stm32.s
)

# Set include paths
target_include_directories(blinky.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/projects/blinky/app
    ${CMAKE_SOURCE_DIR}/common/include
)

target_link_options(blinky.elf PRIVATE "-T${LINKER_SCRIPT}")

add_custom_command(TARGET blinky.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:blinky.elf> ${CMAKE_BINARY_DIR}/blinky.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:blinky.elf> ${CMAKE_BINARY_DIR}/blinky.bin
    COMMENT "Generating HEX & BIN"
)

