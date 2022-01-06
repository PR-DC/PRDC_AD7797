/**
 * LCADC-8_no_lib.ino - Test AD7797 without lib
 * Author: Milos Petrasinovic <mpetrasinovic@pr-dc.com>
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

// Library
// --------------------
#include "SPI.h"

// Define variables
// --------------------
#define NCELL 8
#define ADC1_CS PB3
#define ADC2_CS PB5
#define ADC3_CS PB6
#define ADC4_CS PB7
#define ADC5_CS PB10
#define ADC6_CS PB2
#define ADC7_CS PB1
#define ADC8_CS PA4
#define AD7797_REG_ID 4
#define AD7797_COMM_READ (1 << 6)
#define AD7797_COMM_ADDR(x) (((x) & 0x7) << 3)

const uint8_t adc_cs[] = {ADC1_CS,ADC2_CS,ADC3_CS,ADC4_CS,ADC5_CS,ADC6_CS,ADC7_CS,ADC8_CS};

#define LED_1 PB0
#define LED_2 PB4

// setup function
// --------------------
void setup(){
  // Define pin modes
  for(uint8_t i=0; i<NCELL; i++){
    pinMode(adc_cs[i],OUTPUT);
    digitalWrite(adc_cs[i],HIGH);
  }
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
  
  // Communication settings
  Serial.begin(115200);
  SPI.begin();
  
  // Reset ADC
  for(uint8_t i=0; i<NCELL; i++){
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    selectADC(i);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    unselectADC(i);
    SPI.endTransaction();
    delay(2);
  }
}

// loop function
// --------------------
void loop(){
  for(uint8_t i=0; i<NCELL; i++){
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    selectADC(i);
    Serial.print("ADC ");
    Serial.print(i+1);
    Serial.println(" selected.");
    unsigned char outByte = AD7797_COMM_READ | AD7797_COMM_ADDR(AD7797_REG_ID);
    Serial.println("Checking device ID...");
    SPI.transfer(outByte);
    unsigned char inByte = SPI.transfer(0x00);
    unselectADC(i);
    SPI.endTransaction();
    if((inByte & 0xFF) == 0x5B){
      Serial.println("  AD7797 detected!");
    }
    else{
      Serial.println("Communication error or bad device ID!");
    }
    Serial.println();
    digitalWrite(LED_1,HIGH);
    digitalWrite(LED_2,LOW);
    delay(100);
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,HIGH);
    delay(100);
  }
}

// selectADC function
// Select some ADC
// --------------------
void selectADC(uint8_t nbr){
  digitalWrite(adc_cs[nbr], LOW);
}

// unselectADC function
// Unselect some ADC
// --------------------
void unselectADC(uint8_t nbr){
  digitalWrite(adc_cs[nbr], HIGH);
}
