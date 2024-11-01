# Level Application for Feather RP2040

## Overview
This project implements a Level application using the Feather RP2040 microcontroller. It features an 8x8 LED matrix and an LIS3DH accelerometer to detect tilt. The LED matrix displays four green LEDs to indicate the device is level, or a red LED to indicate the tilt direction if the device is not level.

## Project Structure
- README.md: Information about the Level Application.
- CMakeLists.txt: CMake configuration file needed to build the project.
- pico_sdk_import.cmake: Provides libraries and build system for the RP series microcontroller devices.
- libraries/: Supporting libraries for the LED matrix and other functionality.
  - NeoMatrix.c: Library for 8x8 LED matrix.
  - NeoMatrix.h: Header file with function declarations for NeoMatrix.c.
  - LIS3DH.c: Driver for LIS3DH accelerometer.
  - LIS3DH.h: Header file  with function declarations for LIS3DH.c.
  - neopixel.pio: Allows for pio header generation for loading into a PIO state machine on RP2040.
- src/: Main application code.
  - level.c: Core logic of the Bubble Level application.

## Building the Project
1. Install Dependencies: Make sure CMake and the Pico SDK are set up on your system.
  a. Installing CMake:
    - sudo apt install cmake
  b. Cloning Pico SDK:
    - git clone https://github.com/raspberrypi/pico-sdk

2. Clone Level Project:
  - git clone https://github.com/nicholasmurillo/level
  - cd level

3. Set up Build Directory:
  - mkdir build
  - cd build

4. Run CMake and Build:
  - cmake ..
  - make

5. Load level.uf2 from Build Directory to Device:
  a. Enter BOOTSEL Mode:
    - Connect Feather RP2040 to computer while holding down BOOTSEL button.
  b. Load UF2 to Device:
    - In file explorer, copy level.uf2 executable into RP2040 file system.
