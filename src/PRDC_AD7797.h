/**
 * PRDC_AD7797.h - Analog Devices AD7797 ADC Library
 * Author: Veljko Petrovic <vpetrovic@pr-dc.com>
 * PR-DC, Republic of Serbia
 * info@pr-dc.com
 *
 * --------------------
 * Copyright (C) 2021 PR-DC <info@pr-dc.com>
 *
 * This file is part of PRDC_AD7797.
 *
 * PRDC_AD7797 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * PRDC_AD7797 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PRDC_AD7797.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef __PRDC_AD7797_H__
#define __PRDC_AD7797_H__

#include "Arduino.h"
#include <SPI.h>

// AD7797 Library debug
// #define DEBUG_AD7797

// SPI communication settings
#define AD7797_DEFAULT_SPI_FREQUENCY 1000000
#define AD7797_DEFAULT_SPI SPI
#define AD7797_DEFAULT_CS PIN_SPI_SS
#define AD7797_DEFAULT_MISO PIN_SPI_MISO

// AD7797 Register Map
#define AD7797_REG_COMM		0 // Communications Register(WO, 8-bit)
#define AD7797_REG_STAT	    0 // Status Register	    (RO, 8-bit)
#define AD7797_REG_MODE	    1 // Mode Register	     	(RW, 16-bit
#define AD7797_REG_CONF	    2 // Configuration Register (RW, 16-bit)
#define AD7797_REG_DATA	    3 // Data Register	     	(RO, 16-/24-bit)
#define AD7797_REG_ID	    4 // ID Register	     	(RO, 8-bit)
#define AD7797_REG_OFFSET   6 // Offset Register	    (RW, 24-bit)
#define AD7797_REG_FULLSALE	7 // Full-Scale Register	(RW, 24-bit)

// Communications Register Bit Designations (AD7797_REG_COMM)
#define AD7797_COMM_WEN		(1 << 7) 			// Write Enable
#define AD7797_COMM_WRITE	(0 << 6) 			// Write Operation
#define AD7797_COMM_READ    (1 << 6) 			// Read Operation
#define AD7797_COMM_ADDR(x)	(((x) & 0x7) << 3)	// Register Address
#define AD7797_COMM_CREAD	(1 << 2) 			// Continuous Read of Data Register

// Status Register Bit Designations (AD7797_REG_STAT)
#define AD7797_STAT_RDY		(1 << 7) // Ready
#define AD7797_STAT_ERR		(1 << 6) // Error (Overrange, Underrange)
#define AD7797_STAT_CH3		(1 << 2) // Channel 3
#define AD7797_STAT_CH2		(1 << 1) // Channel 2
#define AD7797_STAT_CH1		(1 << 0) // Channel 1

// Mode Register Bit Designations (AD7797_REG_MODE)
#define AD7797_MODE_SEL(x)		(((x) & 0x7) << 13)	// Operation Mode Select
#define AD7797_MODE_CLKSRC(x)	(((x) & 0x3) << 6) 	// ADC Clock Source Select
#define AD7797_MODE_RATE(x)		((x) & 0xF) 		// Filter Update Rate Select

// AD7797_MODE_SEL(x) options
#define AD7797_MODE_CONT		 0 // Continuous Conversion Mode
#define AD7797_MODE_SINGLE	 1 // Single Conversion Mode
#define AD7797_MODE_IDLE		 2 // Idle Mode
#define AD7797_MODE_PWRDN		 3 // Power-Down Mode
#define AD7797_MODE_CAL_INT_ZERO 4 // Internal Zero-Scale Calibration
#define AD7797_MODE_CAL_INT_FULL 5 // Internal Full-Scale Calibration
#define AD7797_MODE_CAL_SYS_ZERO 6 // System Zero-Scale Calibration
#define AD7797_MODE_CAL_SYS_FULL 7 // System Full-Scale Calibration

// AD7797_MODE_CLKSRC(x) options
#define AD7797_CLK_INT		0 // Internal 64 kHz Clk not available at the CLK pin
#define AD7797_CLK_INT_CO	1 // Internal 64 kHz Clk available at the CLK pin
#define AD7797_CLK_EXT		2 // External 64 kHz Clock
#define AD7797_CLK_EXT_DIV2	3 // External Clock divided by 2

// AD7797_MODE_RATE(x) options
#define AD7797_RATE_123		  0x3 // 123Hz, no rejection
#define AD7797_RATE_62		  0x4 // 62Hz, no rejection
#define AD7797_RATE_50		  0x5 // 50Hz, no rejection
#define AD7797_RATE_33	    0x7 // 33.2Hz, no rejection
#define AD7797_RATE_16_80		0x9 // 16.7Hz, 80dB (60Hz only) rejection
#define AD7797_RATE_16_65		0xA // 16.7Hz, 65dB (50Hz and 60Hz) rejection
#define AD7797_RATE_12		  0xB // 12.5Hz, 66dB (50Hz and 60Hz) rejection
#define AD7797_RATE_10		  0xC // 10Hz, 69dB (50Hz and 60Hz) rejection
#define AD7797_RATE_8		    0xD // 8.33Hz, 70dB (50Hz and 60Hz) rejection
#define AD7797_RATE_6		    0xE // 6.25Hz, 72dB (50Hz and 60Hz) rejection
#define AD7797_RATE_4		    0xF // 4.17Hz, 74dB (50Hz and 60Hz) rejection

// Configuration Register Bit Designations (AD7797_REG_CONF)
#define AD7797_CONF_BO_EN	  (1 << 13) 			// Burnout Current Enable
#define AD7797_CONF_UNIPOLAR  (1 << 12) 			// Unipolar/Bipolar Enable
#define AD7797_CONF_CHAN(x)	  ((x) & 0x7) 			// Channel select


// AD7797_CONF_CHAN(x) options
#define AD7797_CH_AINP_AINM	0 // AIN(+) - AIN(-)
#define AD7797_CH_AINM_AINM	3 // AIN1(-) - AIN1(-)
#define AD7797_CH_TEMP			6 // Temp Sensor
#define AD7797_CH_AVDD_MONITOR	7 // AVDD Monitor

// ID Register Bit Designations (AD7797_REG_ID)
#define AD7797_ID			0x5B
#define AD7797_ID_MASK          0xFF

class PRDC_AD7797 {
  public:
    PRDC_AD7797();

    void setSPIFrequency(uint32_t);
    void setSPI(SPIClass&);

    bool begin();
    bool begin(uint8_t, uint8_t);
    void end();
    void reset();
    void setClockMode(uint8_t);
    void setRate(uint32_t);
    bool checkID();
    void waitReady();
    void setPower(uint8_t);
    void channelSelect(uint8_t);
    void calibrate(uint8_t, uint8_t);
    void setPolarity(uint8_t);
    uint32_t singleConversion();
    uint32_t continuousConversion();
    uint32_t continuousReadAverage(uint32_t);
    // float temperatureRead();
    // float rawToVolts(uint32_t, float);
    void printAllRegisters();

  private:
    SPISettings _spiSettings;
    SPIClass* _spi;
    const uint8_t _gain = 128;
    uint8_t _CS;
    uint8_t _MISO;
    uint8_t _polarity = 0;
    uint8_t _clock_mode = AD7797_CLK_INT;
    uint32_t _rate = AD7797_RATE_123;
    uint8_t _mode = AD7797_MODE_SINGLE;
    void pinInit();
    void beginTransaction();
    void endTransaction();
    uint32_t getRegister(uint8_t, uint8_t);
    uint32_t getSingleRegister(uint8_t, uint8_t);
    void setRegister(uint8_t, uint32_t, uint8_t);
    void setSingleRegister(uint8_t, uint32_t, uint8_t);
};
#endif	// _AD7797_H_
