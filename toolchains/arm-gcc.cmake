cmake_minimum_required(VERSION 3.16)

# Cross context
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Avoid linking on compile-only sanity check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# CPU/ABI
set(MCU_FLAGS "-mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard")
# Or user SoftFP if needed
#set(MCU_FLAGS "-mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=softfp")
# Or no FPU
#set(MCU_FLAGS "-mcpu=cortex-m33 -mthumb -mfloat-abi=soft")

# Common flags
set(COMMON_C_FLAGS "-ffunction-sections -fdata-sections -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables ${MCU_FLAGS}")
set(CMAKE_C_STANDARD 11)

# Include MCU/FPU for try_compile too
set(CMAKE_C_FLAGS "${COMMON_C_FLAGS} ${CMAKE_C_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -Og -g3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2")

set(CMAKE_ASM_FLAGS "${MCU_FLAGS} ${CMAKE_ASM_FLAGS}")

# Linker flags
set(LD_FLAGS "${MCU_FLAGS} -Wl,--gc-sections -Wl,-Map=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")
set(CMAKE_EXE_LINKER_FLAGS "${LD_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")