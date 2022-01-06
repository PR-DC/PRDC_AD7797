/**
 * LCADC-8_test.ino - LCADC-8_test
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

// Library
// --------------------
// PRDC AD7797
// Author: PRDC
#include <PRDC_AD7797.h>

// Defines
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
#define LED_1 PB0
#define LED_2 PB4

// Variables
// --------------------
const uint8_t adc_cs[] = {ADC1_CS,ADC2_CS,ADC3_CS,ADC4_CS,ADC5_CS,ADC6_CS,ADC7_CS,ADC8_CS};
uint32_t t0[NCELL];
uint32_t t_LED;
bool flag_LED = false;

// ADC objects
PRDC_AD7797 AD7797[NCELL];

// setup() function
// --------------------
void setup() {
  // Define pin modes
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
  
  // Communication settings
  Serial.begin(1000000);
  while(!Serial.available()){}
  delay(500);

  // Init ADCs
  for(uint8_t i=0; i<NCELL; i++){
    Serial.print("ADC ");
    Serial.print(i+1);
    Serial.println(" initialization...");
    AD7797[i].setSPI(SPI);
    if(!AD7797[i].begin(adc_cs[i], PIN_SPI_MISO)) {
    Serial.println(F("  AD7797 initialization failed!\n"));
    }
    else{
      AD7797[i].setClockMode(AD7797_CLK_INT);
      AD7797[i].setRate(AD7797_RATE_123);
      AD7797[i].channelSelect(AD7797_CH_AINP_AINM);
      Serial.println(F("  Success!\n"));
    }
    delay(1);
  }
}

// loop() function
// --------------------
void loop(){
  for(uint8_t i=0; i<NCELL; i++){
    Serial.print("ADC ");
    Serial.println(i+1);
    Serial.print("dt = ");
    Serial.print(micros()-t0[i]);
    Serial.println(" us");
    t0[i] = micros();
    Serial.print("value = ");
    Serial.println(AD7797[i].continuousConversion());
    Serial.println();
  }
  blinkLED(1000);
}

// blinkLED() function
// --------------------
void blinkLED(uint16_t interval){
  if((millis()-t_LED)>interval){
    t_LED = millis();
    flag_LED = !flag_LED;
    digitalWrite(LED_1,flag_LED);
    digitalWrite(LED_2,!flag_LED);
  }
}
