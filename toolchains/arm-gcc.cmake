set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# CPU/ABI
set(MCU_FLAGS "-mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard")

# Common flags
set(CMAKE_C_STANDARD 11)
set(COMMON_C_FLAGS "-ffunction-sections -fdata-sections -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables ${MCU_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-Og -g3 ${COMMON_C_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "-O2 ${COMMON_C_FLAGS}")

# Linker flags
set(LINK_FLAGS "${MCU_FLAGS} -specs=nano.specs -specs=nosys.specs")
set(CMAKE_EXE_LINKER_FLAGS "${LINK_FLAGS}")