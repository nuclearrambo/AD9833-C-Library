# AD9833 Library

This is a simple C library to interface with the AD9833 waveform generator chip using an SPI interface.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Features

- Set frequency and phase for the AD9833 waveform generator.
- Control various settings like output mode, sleep mode, reset, etc.
- Perform frequency sweep operation.

## Installation

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/your-username/AD9833-library.git
   ```

2. Copy the `AD9833.c` and `AD9833.h` files into your project directory.

## Usage

1. Include the `AD9833.h` header file in your source file.

   ```c
   #include "AD9833.h"
   ```

2. Initialize the AD9833 device by calling `AD9833_init()` function and passing the SPI handle.

   ```c
   SPI_HandleTypeDef spi; // Replace with your SPI handle
   AD9833_t ad9833;
   AD9833_init(&ad9833, &spi);
   ```

3. Set the frequency and phase using `AD9833_setFrequency()` and `AD9833_setPhase()` functions respectively.

   ```c
   long frequency = 10000; // Replace with the desired frequency value
   AD9833_setFrequency(frequency, &ad9833);

   long phase = 0; // Replace with the desired phase value
   AD9833_setPhase(phase, &ad9833);
   ```

4. Set the output mode of the AD9833 using `AD9833_outputMode()` function.

   ```c
   uint8_t mode = SQUARE; // Replace with the desired output mode (SQUARE or TRIANGLE)
   AD9833_outputMode(&ad9833, mode);
   ```

5. Perform a frequency sweep using the `AD9833_sweep()` function.

   ```c
   uint32_t start_freq = 1000;
   uint32_t stop_freq = 10000;
   int steps = 10;
   int dwell = 100; // 100ms delay between each step

   AD9833_sweep(&ad9833, start_freq, stop_freq, steps, dwell);
 
## Porting to other platform

This library is created to work with the STM32 HAL. To make it work on a different platform, replace the SPI functions with appropriate ones. Also replace the "HAL_" functions wth equivalent ones. 

## Contributing

Contributions are welcome! If you find any issues or want to add new features, feel free to open an issue or submit a pull request.

