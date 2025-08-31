cmake_minimum_required(VERSION 3.16)

project(michaeldello-embedded C CXX ASM)

# Toggle what to build
option(BUILD_FIRMWARE "Build embedded firmware targets" OFF)
option(UNIT_TESTS     "Build unit tests"                ON)

# TBD - Pass a toolchain file for BUILD_FIRMWARE=ON
# e.g., -DCMAKE_TOOLCHAIN_FILE=path/to/arm-none-eabi-toolchain.cmake

# Firmware targets
if(BUILD_FIRMWARE)
  add_subdirectory(projects/blinky)
endif()

# Unit tests (all)
if(UNIT_TESTS)
  include(CTest)
  add_subdirectory(unit_tests)
endif()