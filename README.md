# michaeldello-embedded - Overview

This repo contains Embedded C projects for deployment to an STM32 development target. STM32 is what I have in my home lab from previous projects, and is a popular platform. The purpose of this is to build and maintain a series of reference projects with documentation for future reference for anyone (well, me) developing a new embedded software system.

These software projects are developed using modern methods and tools:
- IDEs such as STM32Cube and VSCode, and their rich set of libraries and plug-ins
- Version controlled Docker Container(s) for a traceable common build, quality scan, and unit test environment
- Git + CI (GitHub Actions) to version control and automate development and integration

These projects are initially limited to blinky, USART/UART, and SPI examples.

# Project Organization

## Execution Environments

Projects span multiple contexts, not only the application tech (LED, UART, SPI, etc...), but there is separate support for bare metal, HAL/LL, FreeRTOS, and Embedded Linux. These environments may be deployed to newer STM32 technology over time, so a reusable scalable architecture is desirable. Therefore, application code should be decoupled from platform level details, with MCU family details encapsulated into their own subfolders. Build systems per project can then be parameterized. Application code can then be implemented using common interfaces to underlying platform details, with the build system building the right components based on the target requested.

## Example Directory Structure

The following directory structure supports an architecture that promotes hardware platform decoupling, unit testing, and a coherent CMake strategy. Platform and target board technology can be linked in based on the tope level build target without needing messy if/else decision trees.

```
michaeldello-embedded/
├── config/
│   ├─── blinky.baremetal.stm32h5.mk
│   └─── uart.hal.stm32h7.mk
├── common/
│   ├── drivers/
│   │   └── usart/
│   │       ├── usart_generic.c
│   │       ├── usart_stm32h5.c
│   │       ├── usart_stm32h7.c
│   │       └── ...
│   ├── include/
│   │   ├── gpio_api.h
│   │   └── ...
│   ├── linker/
│   │   ├── stm32h5/
│   │   └── stm32h7/
│   ├── platform/
│   │   ├── baremetal/
│   │   │   ├── stm32h5/
│   │   │   └── stm32h7/
│   │   ├── hal/
│   │   │   ├── stm32h5/
│   │   │   └── stm32h7/
│   │   ├── freertos/
│   │   │   ├── stm32h5/
│   │   │   └── stm32h7/
│   │   └── linux/
│   ├── startup/
│   │   ├── stm32h5/
│   │   └── stm32h7/
│   └── unit_tests/
│       └── stubs/
│           └── gpio_stub.c
├── docs/
├── projects/
│   ├── blinky/
│   │   ├── app/
│   │   ├── CMakeLists.txt
│   │   ├── main.c
│   │   └── unit_tests/
│   │       ├── test_blinky.cpp
│   │       └── CMakeLists.txt
│   ├── uart/
│   └── spi/
└── tools/
│   └── flash.sh
├── unit_tests/
│   └── CMakeLists.txt
```

## CMake Strategy

1. Each project contains its own application and unit tests
1. The top level CMakeLists.txt discovers and builds them separately
1. Each project main links in the target platform and board implementations
1. Each project unit test main links in hardware and platform stubs

## Example Build Config

Project makefiles are configurable for MCU Family. 
```
FAMILY ?= stm32h5

STARTUP_DIR = ../../common/startup/$(FAMILY)
LINKER_DIR = ../../common/linker/$(FAMILY)
PLATFORM_DIR = ../../common/platform/baremetal/$(FAMILY)
```

# Development Environment

This repo is set up for using modern embedded development technology, tools, and methods.

## Mac Local Host

The local host development environment is on a Mac with Apple Silicon M2 chipset. This is important for VSCode IDE, for example.

## Docker Containerization

The build environment is containerized using Docker. This allows the build and test execution environment to remain stable and independent of the underlying host tech, which could be either a local host machine or a runner in GitHub Actions infrastructure. 

Containerizing the build environment also allows the development environment to be version controlled, allowing pin, upgrade, or reversion as needed.

The docker container is based on an Ubuntu version that supports multiple architectures, however, this repo will be built and tested on ARM platforms. Other platform tech may require tweaks to configuration files and container build instructions.

To support development in VSCode, a .devcontainer is used to integrate the Docker container for execution and debug.

## GNU ARM Toolchain

The GNU ARM Toolchain should be pinned to a particular version rather than installing it as an Ubuntu apt package so that this doesn't change down the road if the container is ever rebuilt.

## Folder Structure

The following files and folder structure support the local and CI build and test development environment.
```
michaeldello-embedded/
├── docker/
│   └── Dockerfile
├── .devcontainer/
│   └── devcontainer.json
├── .github/
│   └── workflows/
│       └── ci.yml
├── ...
```

# Maintenance and Extension

## Adding a new Application

Address these items to add a new application (or extend and existing one):

1. Create a new, named application folder under projects/
1. Add a README.md for preserving important notes
1. Add an application header file in an app/ folder under the named application folder
1. Iterate on the implementation module and unit tests
    1. TDD can be used here to ensure a decoupled approach that can be iterated
1. Create a main program for the new application's execution under the named application's folder that exercises the application's implementation
1. Create/extend the target platform/board linker file(s)
1. Create a CMakeLists.txt in the named application folder
1. Get the application building in Docker

## Building Applications

### Building in Docker

Run the Docker build container:

```
docker run --rm -it -v $(pwd):/workspace -w /workspace ghcr.io/michaeldello/stm32-dev:latest bash
```

This can also be opened in the context of VSCode.

Build the Release:

```
rm -rf build-fw
cmake -S . -B build-fw \
  -DBUILD_FIRMWARE=ON \
  -DUNIT_TESTS=OFF \
  -DCMAKE_TOOLCHAIN_FILE=toolchains/arm-gcc.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-fw --parallel
```

## Loading the Application

Using ST-Link:
```
openocd -f interface/stlink.cfg -f target/stm32h5x.cfg \
  -c "program build-fw/blinky.elf verify reset exit"
```

Using J-Link:
```
openocd -f interface/jlink.cfg -f target/stm32h5x.cfg \
  -c "program build-fw/blinky.elf verify reset exit"
```

Using SEGGER GDB:
```
JLinkGDBServer -device STM32H563ZI -if SWD -speed 4000
arm-none-eabi-gdb build-fw/blinky.elf
(gdb) target remote :2331
(gdb) monitor reset halt
(gdb) load
(gdb) continue
```