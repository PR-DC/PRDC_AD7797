/**
 * PRDC_AD7797.cpp - Analog Devices AD7797 ADC Library
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


 #include "PRDC_AD7797.h"

 // PRDC_AD7797()
 // Object constructor
 // --------------------
 PRDC_AD7797::PRDC_AD7797() :
   _spiSettings(AD7797_DEFAULT_SPI_FREQUENCY, MSBFIRST, SPI_MODE3),
   _spi(&AD7797_DEFAULT_SPI), _CS(AD7797_DEFAULT_CS), _MISO(AD7797_DEFAULT_MISO) {

 }

 // setSPI() function
 // Set SPI object
 // --------------------
 void PRDC_AD7797::setSPI(SPIClass& spi) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setSPI()"));
   #endif

   _spi = &spi;
 }

 // setSPIFrequency() function
 // Set SPI frequency
 // --------------------
 void PRDC_AD7797::setSPIFrequency(uint32_t frequency) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setSPIFrequency()"));
   #endif

   _spiSettings = SPISettings(frequency, MSBFIRST, SPI_MODE3);
 }

 // beginTransaction() function
 // Begin SPI transaction
 // --------------------
 inline void PRDC_AD7797::beginTransaction() {
   _spi->beginTransaction(_spiSettings);
   digitalWrite(_CS, LOW);
 }

 // endTransaction() function
 // End SPI transaction
 // --------------------
 inline void PRDC_AD7797::endTransaction() {
   digitalWrite(_CS, HIGH);
   _spi->endTransaction();
 }

 // getRegister() function
 // Reads the value of a register
 // --------------------
 uint32_t PRDC_AD7797::getRegister(uint8_t registerAddress, uint8_t bytesNumber) {
   #ifdef DEBUG_AD7797
     Serial.println(F("getRegister()"));
     Serial.print(F("Register "));
     Serial.println(registerAddress);
   #endif

   uint32_t buffer = 0x0;
   _spi->transfer(AD7797_COMM_READ | AD7797_COMM_ADDR(registerAddress));
   for(uint8_t i = 0; i < bytesNumber; i++)  {
       buffer = (buffer << 8) + _spi->transfer(0);
   }

   #ifdef DEBUG_AD7797
     Serial.println(buffer);
   #endif
   return buffer;
 }

 // getSingleRegister() function
 // Reads the value of a single register
 // --------------------
 uint32_t PRDC_AD7797::getSingleRegister(uint8_t registerAddress, uint8_t bytesNumber) {
   #ifdef DEBUG_AD7797
     Serial.println(F("getSingleRegister()"));
   #endif

   this->beginTransaction();
   uint32_t regVal = this->getRegister(registerAddress, bytesNumber);
   this->endTransaction();
   return regVal;
 }

 // setRegister() function
 // Set the value of a register
 // --------------------
 void PRDC_AD7797::setRegister(uint8_t registerAddress, uint32_t value, uint8_t bytesNumber) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setRegister()"));
     Serial.print(F("Register "));
     Serial.print(registerAddress);
     Serial.print(F(" Value "));
     Serial.println(value);
   #endif

   uint8_t* dataPointer = (uint8_t*)&value;
   _spi->transfer(AD7797_COMM_WRITE | AD7797_COMM_ADDR(registerAddress));
   for(uint8_t i = 0; i < bytesNumber; i++)  {
     _spi->transfer(*(dataPointer + (bytesNumber-1) - i));
   }
 }

 // setSingleRegister() function
 // Set the value of a single register
 // --------------------
 void PRDC_AD7797::setSingleRegister(uint8_t registerAddress, uint32_t value, uint8_t bytesNumber) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setSingleRegister()"));
   #endif

   this->beginTransaction();
   this->setRegister(registerAddress, value, bytesNumber);
   this->endTransaction();
 }

 // begin() function
 // Begin communication
 // --------------------
 bool PRDC_AD7797::begin() {
   #ifdef DEBUG_AD7797
     Serial.println(F("begin()"));
     Serial.println(F("PRDC_AD7797 constructor instantiated successfully"));
     Serial.print(F("CS "));
     Serial.println(_CS);
     Serial.print(F("MISO "));
     Serial.println(_MISO);
   #endif

   _spi->begin();
   this->pinInit();
   this->reset();
   if(this->checkID()) {
     return true;
   }
   return false;
 }

 bool PRDC_AD7797::begin(uint8_t CS, uint8_t MISO) {
   _CS = CS;
   _MISO = MISO;
   return this->begin();
 }

 // end() function
 // End communication
 // --------------------
 void PRDC_AD7797::end() {
   #ifdef DEBUG_AD7797
     Serial.println(F("end()"));
   #endif

   _spi->end();
 }

 // pinInit() function
 // Pin initialization
 // --------------------
 void PRDC_AD7797::pinInit() {
   #ifdef DEBUG_AD7797
     Serial.println(F("pinInit()"));
   #endif

   pinMode(_CS, OUTPUT);
   digitalWrite(_CS, HIGH);
 }

 // reset() function
 // Reset ADC
 // --------------------
 void PRDC_AD7797::reset() {
   #ifdef DEBUG_AD7797
     Serial.println(F("reset()"));
   #endif

   this->beginTransaction();
   _spi->transfer(0xFF);
   _spi->transfer(0xFF);
   _spi->transfer(0xFF);
   _spi->transfer(0xFF);
   this->endTransaction();
   delay(1);
 }

 // setClockMode() function
 // Set clock mode (internal or external)
 // --------------------
 void PRDC_AD7797::setClockMode(uint8_t clock_mode) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setClockMode()"));
   #endif

   _clock_mode = clock_mode;
 }

 // setRate() function
 // Set output data rate
 // --------------------
 void PRDC_AD7797::setRate(uint32_t rate) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setRate()"));
   #endif

   _rate = rate;
 }

 // checkID() function
 // Check ADC ID register
 // --------------------
 bool PRDC_AD7797::checkID() {
   #ifdef DEBUG_AD7797
     Serial.println(F("checkID()"));
   #endif

   uint8_t regVal = getSingleRegister(AD7797_REG_ID, 1);
   #ifdef DEBUG_AD7797
     Serial.print(F("ID = "));
     Serial.println(regVal & AD7797_ID_MASK);
   #endif
   return (regVal & AD7797_ID_MASK) == AD7797_ID;
 }

 // waitReady() function
 // Waits for MISO pin to go low
 // --------------------
 void PRDC_AD7797::waitReady() {
   #ifdef DEBUG_AD7797
     Serial.println(F("waitReady()"));
   #endif
   while(digitalRead(_MISO)) {}
 }

 // setPower() function
 // Set device to idle or power-down
 // mode =
 //  0 - power-down
 //  1 - idle
 // --------------------
 void PRDC_AD7797::setPower(uint8_t mode) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setPower()"));
   #endif

    uint32_t oldPwrMode = this->getSingleRegister(AD7797_REG_MODE, 2);
    oldPwrMode &= ~(AD7797_MODE_SEL(0x7));
    uint32_t newPwrMode = oldPwrMode |
                          AD7797_MODE_SEL((mode * (AD7797_MODE_IDLE)) |
                          (!mode * (AD7797_MODE_PWRDN)));
    this->setSingleRegister(AD7797_REG_MODE, newPwrMode, 3);
 }

 // channelSelect() function
 // Selects the channel to be enabled
 // --------------------
 void PRDC_AD7797::channelSelect(uint8_t channel) {
   #ifdef DEBUG_AD7797
     Serial.println(F("channelSelect()"));
   #endif
   uint32_t regValue = this->getSingleRegister(AD7797_REG_CONF, 2);
   regValue &= ~AD7797_CONF_CHAN(0xFF);
   regValue |= AD7797_CONF_CHAN(channel);
   this->setSingleRegister(AD7797_REG_CONF, regValue, 2);
 }

 // calibrate() function
 // Performs the given calibration to the specified channel
 // --------------------
 void PRDC_AD7797::calibrate(uint8_t mode, uint8_t channel) {
   #ifdef DEBUG_AD7797
     Serial.println(F("calibrate()"));
   #endif

   this->channelSelect(channel);
   uint32_t regValue = this->getSingleRegister(AD7797_REG_MODE, 2);
   regValue &= ~AD7797_MODE_SEL(0x7);
   regValue |= AD7797_MODE_SEL(mode);
   this->beginTransaction();
   this->setRegister(AD7797_REG_MODE, regValue, 2);
   this->waitReady();
   this->endTransaction();
 }

 // setPolarity() function
 // Selects the polarity of the coding
 // --------------------
 void PRDC_AD7797::setPolarity(uint8_t polarity) {
   #ifdef DEBUG_AD7797
     Serial.println(F("setPolarity()"));
   #endif

   uint32_t regValue = this->getSingleRegister(AD7797_REG_CONF, 2);
   regValue &= ~AD7797_CONF_UNIPOLAR;
   regValue |= polarity * AD7797_CONF_UNIPOLAR;
   this->setSingleRegister(AD7797_REG_CONF, regValue, 2);
   _polarity = polarity;
 }

 // singleConversion() function
 // Returns the result of a single conversion
 // --------------------
 uint32_t PRDC_AD7797::singleConversion() {
   #ifdef DEBUG_AD7797
     Serial.println(F("singleConversion()"));
   #endif

   uint32_t command = AD7797_MODE_SEL(AD7797_MODE_SINGLE) |
                      AD7797_MODE_CLKSRC(_clock_mode) |
                      AD7797_MODE_RATE(_rate);
   _mode = AD7797_MODE_SINGLE;
   this->beginTransaction();
   this->setRegister(AD7797_REG_MODE, command, 2);
   this->waitReady();
   uint32_t raw = this->getRegister(AD7797_REG_DATA, 3);
   this->endTransaction();

   return raw;
 }

 // continuousConversion() function
 // Returns the result of continuous conversion
 // --------------------
 uint32_t PRDC_AD7797::continuousConversion() {
   #ifdef DEBUG_AD7797
     Serial.println(F("continuousConversion()"));
   #endif
   if(_mode != AD7797_MODE_CONT){
     uint32_t command = AD7797_MODE_SEL(AD7797_MODE_CONT) |
                        AD7797_MODE_CLKSRC(_clock_mode) |
                        AD7797_MODE_RATE(_rate);
     _mode = AD7797_MODE_CONT;
     this->beginTransaction();
     this->setRegister(AD7797_REG_MODE, command, 2);
     this->waitReady();
     uint32_t raw = this->getRegister(AD7797_REG_DATA, 3);
     this->endTransaction();
     return raw;
   }
   else{
     this->beginTransaction();
     this->waitReady();
     uint32_t raw = this->getRegister(AD7797_REG_DATA, 3);
     this->endTransaction();
     return raw;
   }
 }

 // continuousReadAverage() function
 // Returns the average of several conversion results
 // --------------------
 uint32_t PRDC_AD7797::continuousReadAverage(uint32_t sampleNumber) {
   #ifdef DEBUG_AD7797
     Serial.println(F("continuousReadAverage()"));
   #endif

   uint32_t samplesAverage = 0;
   uint32_t command = AD7797_MODE_SEL(AD7797_MODE_CONT) |
                      AD7797_MODE_CLKSRC(_clock_mode) |
                      AD7797_MODE_RATE(_rate);

   this->beginTransaction();
   this->setRegister(AD7797_REG_MODE, command, 2);
   for(uint32_t i = 0; i < sampleNumber; i++) {
     this->waitReady();
     samplesAverage += this->getRegister(AD7797_REG_DATA, 3);
   }
   this->endTransaction();

   return samplesAverage/sampleNumber;
 }

 // // temperatureRead() function
 // // Read data from temperature sensor and converts it to Celsius degrees
 // // --------------------
 // float PRDC_AD7797::temperatureRead() {
 //   #ifdef DEBUG_AD7797
 //     Serial.println(F("temperatureRead()"));
 //   #endif
 //
 //   this->setPolarity(0); // Bipolar operation
 //   this->channelSelect(AD7797_CH_TEMP);
 //   uint32_t dataReg = this->singleConversion();
 //   dataReg -= 0x800000;
 //   return (float)dataReg/2815.0-273; // Celsius Temperature
 // }

 // rawToVolts() function
 // Converts 24-bit raw data to volts
 // --------------------
 // float PRDC_AD7797::rawToVolts(uint32_t raw, float vRef) {
 //   #ifdef DEBUG_AD7797
 //     Serial.println(F("rawToVolts()"));
 //   #endif
 //
 //   float voltage;
 //   if(_polarity == 0) {
 //     // Bipolar mode
 //     voltage = (((float)raw/(1ul << 23))-1)*vRef/_gain;
 //   } else {
 //     // Unipolar mode
 //     voltage = ((float)raw*vRef)/(1ul << 24)/_gain;
 //   }
 //
 //   return voltage;
 // }

 // printAllRegisters() function
 // Print value of each register
 // --------------------
 void PRDC_AD7797::printAllRegisters(void)  {
   Serial.println(F("Read all registers"));
   this->beginTransaction();
   Serial.println(F("Register 0"));
   Serial.println(this->getRegister(0, 1));
   Serial.println(F("Register 1"));
   Serial.println(this->getRegister(1, 3));
   Serial.println(F("Register 2"));
   Serial.println(this->getRegister(2, 3));
   Serial.println(F("Register 3"));
   Serial.println(this->getRegister(3, 3));
   Serial.println(F("Register 4"));
   Serial.println(this->getRegister(4, 1));
   Serial.println(F("Register 5"));
   Serial.println(this->getRegister(5, 1));
   Serial.println(F("Register 6"));
   Serial.println(this->getRegister(6, 3));
   Serial.println(F("Register 7"));
   Serial.println(this->getRegister(7, 3));
   this->endTransaction();
 }
