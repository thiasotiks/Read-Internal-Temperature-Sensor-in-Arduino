/*
 * Copyright (c) 2021 Sayantan Sinha
 *
 * MIT License
 */

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);                      // Connect 16x2 LCD through I2C (address = 0x3F)

unsigned int readADC(byte ch);                           // Own function to read ADC

void setup()
{
  ADMUX = 0b11000000;                                    // Select V_ref = internal 1.1 V [Ref. ATmega328P datasheet, p. 317]
  ADCSRA = 0b10000111;                                   // Enable ADC, Set prescaler = 128. (ADC_frequency = f_CPU / prescaler)
  lcd.begin();
}

void loop()
{
  int a = readADC(8);                                    // Read internal temperature sensor value (connected to ADC channel #8)
  int t = 33 + ((float)(a - 363) / 1.074);               // Emperical formula to calculate temperature (in Celsius) from ADC reading. (from observation: ADC reading = 363 @ 33℃)
  lcd.clear();
  lcd.print(t);
  delay(1000);
}

unsigned int readADC(byte ch)
{
  if (ch > 15)                                           // If the ADC channel selector is more than 15 then it's invalid
    return 0;
  ADMUX &= 0xF0;                                         // Clear the old ADC channel multiplexer bits
  ADMUX |= ch;                                           // Load the ADC channel multiplexer bits
  ADCSRA |= 0x40;
  while (ADCSRA & 0x40);                                 // The ADC start conversion bit remains HIGH until the ongoing A/D conversion is finished
  unsigned int aRead = ADCL;                             // Read the ADC result (lower byte first)
  aRead |= (ADCH << 8);                                  // Read higher 2 bits (bit-9, bit-8) of the ADC result
  return (aRead);
}
