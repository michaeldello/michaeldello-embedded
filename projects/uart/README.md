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

The hardware drivers for different platforms are found in platform/baremetal/

## Build and Unit Test

To build and run unit tests in the Docker container, use the following commands from the repository root dir:
```
rm -rf build && cmake -S . -B build -DUNIT_TESTS=ON -DBUILD_FIRMWARE=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure -L uart
```