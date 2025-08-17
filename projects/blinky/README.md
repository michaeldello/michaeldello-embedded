# Blinky Project Plan

- Define a small, reusable GPIO API that’s independent of STM32
- Implement portable Blinky application logic: GPIO set and toggle
- Write unit tests for set and toggle logic using stubbed hardware layer
- Add stm32h5 bare-metal GPIO and configure for STM32H563 NUCLEO board LED to blink LED wired to GPIO B
- Run on hardware and observe blinking LED