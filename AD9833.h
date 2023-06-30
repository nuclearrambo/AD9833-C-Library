#include <stdlib.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"
#define MCLK 25000000.0

enum dds_modes{
	SQUARE = 0,
	TRIANGLE
};



typedef union {
	uint16_t ctrl;
	struct{
		uint8_t reserved :1;
		uint8_t mode		 :1;
		uint8_t reserved1:1;
		uint8_t div2		 :1;
		uint8_t reserved3:1;
		uint8_t opbiten  :1;
		uint8_t sleep12  :1;
		uint8_t sleep1	 :1;
		uint8_t reset		 :1;
		uint8_t reserved4:1;
		uint8_t pselect  :1;
		uint8_t fselect  :1;
		uint8_t hlb			 :1;
		uint8_t b28			 :1;
		uint8_t d14			 :1;
		uint8_t d15			 :1;
	}bit;
} CtrlReg;

typedef union {
	uint32_t freq;
	struct{
		uint32_t freq_L	:14;
		uint32_t address0:2;
		uint32_t freq_H :14;
		uint32_t address1:2;
	}bit;
} FreqReg;

typedef union {
	uint32_t phase;
	struct{
		uint32_t phase	:28;
		uint32_t pad		:4;
	}bit;
}	PhaseReg;

typedef struct {
    SPI_HandleTypeDef* dev; // Pointer to SPI handle type
    long frequency; // Frequency setting for the waveform generator
    long phase; // Phase offset setting for the waveform generator
    CtrlReg ctrlReg; // Control register of the AD9833
    FreqReg freqReg; // Frequency register of the AD9833
    PhaseReg phaseReg; // Phase register of the AD9833
} AD9833_t;


void AD9833_calculateFreqReg(long f, FreqReg *reg);
void AD9833_init(AD9833_t *ad9833, SPI_HandleTypeDef *spi);
void writeSPI(SPI_HandleTypeDef *dev, uint8_t *data, uint8_t bytes);
void AD9833_setFrequency(long f, AD9833_t *ad9833);
void AD9833_setConfig(AD9833_t *ad9833);
void AD9833_writeSPI(SPI_HandleTypeDef *dev, uint8_t *data, uint8_t bytes);
void AD9833_sweep(AD9833_t *ad9833, uint32_t start_freq, uint32_t stop_freq, int steps, int dwell);

