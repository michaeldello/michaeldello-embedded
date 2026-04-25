# Blinky Project Plan

- Define a small, reusable GPIO API that’s independent of STM32
- Implement portable Blinky application logic: GPIO set and toggle
- Write unit tests for set and toggle logic using stubbed hardware layer
- Add stm32h5 bare-metal GPIO and configure for STM32H563 NUCLEO board LED to blink LED wired to GPIO B
- Run on hardware and observe blinking LED

# Target Hardware Test

These steps target an STM32H563ZI NUCLEO/ZI development board connected to the
host over its ST-LINK USB port.

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

The blinky ELF is generated at:

```
build-fw/projects/blinky/blinky.elf
```

## Flash With ST-LINK

Run this from the repository root on the host machine connected to the board:

```
openocd -f interface/stlink.cfg -f target/stm32h5x.cfg \
  -c "program build-fw/projects/blinky/blinky.elf verify reset exit"
```

## Verify

After programming and reset, the configured board LED should blink. If it does
not, confirm the board is powered from the ST-LINK USB connector and that the
LED GPIO configuration in `common/platform/baremetal/stm32h5/platform_config.h`
matches the board revision.
