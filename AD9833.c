#include <stdlib.h>
#include <stdint.h>
#include "AD9833.h"
#include "main.h"

/**
 * Calculate the frequency register settings for the given frequency value.
 *
 * @param f The frequency value for which to calculate the frequency register settings.
 * @param reg Pointer to FreqReg structure to store the calculated frequency register settings.
 */
void AD9833_calculateFreqReg(long f, FreqReg* reg) {
    long _freqReg;
    _freqReg = (f * 268435456.0) / MCLK; // Calculate the frequency register value using the given frequency and MCLK value

    reg->bit.address0 = 0x01; // Set the address0 bits in the frequency register
    reg->bit.address1 = 0x01; // Set the address1 bits in the frequency register
    reg->bit.freq_L = _freqReg & 0x3FFF; // Set the lower 14 bits of the frequency register
    reg->bit.freq_H = (_freqReg >> 14) & 0x3FFF; // Set the upper 14 bits of the frequency register
}

/**
 * Initialize the AD9833 waveform generator.
 *
 * @param ad9833 Pointer to AD9833_t structure representing the AD9833 device.
 * @param spi Pointer to SPI handle for communication with the AD9833.
 */
void AD9833_init(AD9833_t *ad9833, SPI_HandleTypeDef *spi) {
    ad9833->dev = spi; // Assign the SPI handle to the AD9833 device structure
    ad9833->ctrlReg.ctrl = 0x0000; // Initialize the control register to 0x0000

    ad9833->ctrlReg.bit.reset = 0x1; // Set the reset bit to initiate reset
    AD9833_setConfig(ad9833); // Apply the configuration by calling AD9833_setConfig function

    ad9833->ctrlReg.bit.b28 = 1; // Set the b28 bit to enable 28-bit frequency control
    ad9833->ctrlReg.bit.reset = 0; // Clear the reset bit to exit reset state

    HAL_Delay(1); // Delay for 1 millisecond

    AD9833_setConfig(ad9833); // Apply the configuration by calling AD9833_setConfig function

    HAL_Delay(1); // Delay for 1 millisecond

    AD9833_setConfig(ad9833); // Apply the configuration by calling AD9833_setConfig function
}

/**
 * Set the frequency of the AD9833 waveform generator.
 *
 * @param f The frequency value to be set.
 * @param ad9833 Pointer to AD9833_t structure representing the AD9833 device.
 */
void AD9833_setFrequency(long f, AD9833_t *ad9833) {
    AD9833_calculateFreqReg(f, &ad9833->freqReg); // Calculate frequency register settings based on the given frequency

    uint8_t bytes[2];
    uint32_t unit = ad9833->freqReg.freq;

    bytes[0] = (unit >> 8) & 0xFF; // Extract the high byte of the frequency register
    bytes[1] = unit & 0xFF; // Extract the low byte of the frequency register
    AD9833_writeSPI(ad9833->dev, bytes, sizeof(bytes)); // Write the frequency bytes to the AD9833 via SPI

    unit = (ad9833->freqReg.freq >> 16);
    bytes[0] = (unit >> 8) & 0xFF;
    bytes[1] = unit & 0xFF;
    AD9833_writeSPI(ad9833->dev, bytes, sizeof(bytes));
}

/**
 * Set the configuration of the AD9833 waveform generator.
 *
 * @param ad9833 Pointer to AD9833_t structure representing the AD9833 device.
 */
void AD9833_setConfig(AD9833_t *ad9833) {
    uint8_t bytes[2];
    uint32_t unit = ad9833->ctrlReg.ctrl;

    bytes[0] = (unit >> 8) & 0xFF;
    bytes[1] = unit & 0xFF;
    AD9833_writeSPI(ad9833->dev, bytes, sizeof(bytes));
}

/**
 * Set the output mode of the AD9833 waveform generator.
 *
 * @param ad9833 Pointer to AD9833_t structure representing the AD9833 device.
 * @param mode The output mode to be set.
 */
void AD9833_outputMode(AD9833_t *ad9833, uint8_t mode) {
    ad9833->ctrlReg.bit.mode = mode; // Set the output mode in the control register
    AD9833_setConfig(ad9833); // Apply the updated configuration to the AD9833
}

/**
 * Perform a frequency sweep on the AD9833 waveform generator.
 *
 * @param ad9833 Pointer to AD9833_t structure representing the AD9833 device.
 * @param start_freq The starting frequency of the sweep.
 * @param stop_freq The stopping frequency of the sweep.
 * @param steps The number of steps in the sweep.
 * @param dwell The duration of each step in milliseconds.
 */
void AD9833_sweep(AD9833_t *ad9833, uint32_t start_freq, uint32_t stop_freq, int steps, int dwell) {
    uint32_t delta_frequency = (stop_freq - start_freq) / steps;
    uint32_t current_step = 0;

    while (current_step <= steps) {
        AD9833_setFrequency(start_freq + delta_frequency * current_step, ad9833); // Set the frequency for the current step
        HAL_Delay(dwell); // Delay for the specified dwell time
        current_step += 1; // Increment the current step
    }
}

void AD9833_writeSPI(SPI_HandleTypeDef *dev, uint8_t *data, uint8_t bytes){
	HAL_GPIO_WritePin(FSYNC_9833_GPIO_Port, FSYNC_9833_Pin, GPIO_PIN_RESET);
	for(int j=0; j<bytes; j++){
		HAL_SPI_Transmit(dev, data+j, 1, 10);
	}
	HAL_GPIO_WritePin(FSYNC_9833_GPIO_Port, FSYNC_9833_Pin, GPIO_PIN_SET);
}
