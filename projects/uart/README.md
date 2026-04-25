# UART Project Plan

- Define small, reusable UART API independent of STM32
- Implement portable UART core: ring buffer + “echo task” that moves bytes from RX FIFO to TX when space exists
- Write unit tests for buffer and echo behavior using stubbed hardware layer
- Add stm32h5 bare-metal UART (one instance) and wire it to the ST-LINK VCP pins (configurable)
- Run on hardware and prove echo over the USB VCP

# Code Layout

## Portable Core

Portable UART code is found in common/drivers/uart/

## Hardware Driver

The hardware drivers for different platforms are selected by the build and found
under the appropriate common/platform backend directory, such as baremetal, HAL,
or RTOS.

## Build and Unit Test

To build and run unit tests in the Docker container, use the following commands from the repository root dir:
```
rm -rf build && cmake -S . -B build -DUNIT_TESTS=ON -DBUILD_FIRMWARE=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure -L uart
```

# Target Hardware Test

These steps target an STM32H563ZI NUCLEO/ZI development board connected to the
host over its ST-LINK USB port. The bare-metal STM32H5 backend is configured for
USART3 on `PD8` TX and `PD9` RX with alternate function 7, which is the
ST-LINK virtual COM port routing on the target board.

## Build Firmware

From the repository root, build the firmware image:

```
rm -rf build-fw
cmake -S . -B build-fw \
  -DBUILD_FIRMWARE=ON \
  -DUNIT_TESTS=OFF \
  -DCMAKE_TOOLCHAIN_FILE=toolchains/arm-gcc.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-fw --parallel
```

The UART echo ELF is generated at:

```
build-fw/projects/uart/uart_echo.elf
```

## Flash With ST-LINK

Run this from the repository root on the host machine connected to the board:

```
openocd -f interface/stlink.cfg -f target/stm32h5x.cfg \
  -c "program build-fw/projects/uart/uart_echo.elf verify reset exit"
```

## Open Serial Terminal

The UART echo application uses `115200 8N1`.

On macOS, find the ST-LINK virtual COM port and open it with `screen`:

```
ls /dev/tty.usbmodem*
screen /dev/tty.usbmodemXXXX 115200
```

Replace `/dev/tty.usbmodemXXXX` with the device path found by `ls`. To leave
`screen`, press `Ctrl-A`, then `K`, then confirm with `Y`.

On Linux, the ST-LINK virtual COM port commonly appears as `/dev/ttyACM0`:

```
ls /dev/ttyACM*
picocom -b 115200 /dev/ttyACM0
```

## Verify

Type characters in the serial terminal. Each received byte should echo back over
the same terminal. If no characters echo, confirm the board is powered from the
ST-LINK USB connector, the serial terminal is connected to the ST-LINK virtual
COM port, and the UART pin/clock settings in
`common/platform/baremetal/stm32h5/platform_config.h` match the board revision.
