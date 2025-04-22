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
```
michaeldello-embedded/
├── config/
│   ├─── blinky.baremetal.stm32h5.mk
│   └─── uart.hal.stm32h7.mk
├── common/
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
│   ├── linker/
│   │   ├── stm32h5/
│   │   └── stm32h7/
│   └── drivers/
│       ├── usart/
│           ├── usart_generic.c
│           ├── usart_stm32h5.c
│           └── usart_stm32h7.c
├── docs/
├── projects/
│   ├── blinky/
│   │   ├── app/
│   │   └── Makefile
│   ├── uart/
│   └── spi/
└── tools/
    └── flash.sh
```

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