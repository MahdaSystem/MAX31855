/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX31855_H
#define __MAX31855_H

/*
  Supply Voltage Range (VCC to GND) -0.3V to +4.0V
  0.25°C Resolution Converter
  MAX31855 maximum supported spi clock frequency is 5MHz.
  MAX31855 maximum convert time is 100ms.
  
  The SpiDriver delay must be atleast 200ns according to
  <Serial-Interface Timing Characteristics> in page 4 of datasheet.
  
  Spi Settings:
  Data size: 8 bits.
  First bit: MSB first.
  Clock Polarity (CPOL): Low
  Clock Phase (CPHA): 1 Edge
  
  For Thermal Characteristics refer to Page 3 of datasheet.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct MAX31855_SpiDriver_Struct {
  void (*Max31855_CS_Low)(void);
  void (*Max31855_CS_High)(void);
  uint8_t (*Max31855_Spi_ReadByte)(void);
  void (*Max31855_Delay_1ms)(void);
} MAX31855_SpiDriver_Typedef;

typedef struct MAX31855_Data_Struct {
  uint8_t RawDataArray[4];
  uint32_t RawData;
  bool OC_Fault;
  bool SCG_Fault;
  bool SCV_Fault;
  bool Fault;
  float ReferenceJunctionTemperature;
  float ThermocoupleTemperatureData;
} MAX31855_Data_Typedef;
  
bool MAX31855_Init(MAX31855_SpiDriver_Typedef * max31855_SpiDriver,
 void (*Max31855_CS_Low)(void),
 void (*Max31855_CS_High)(void),
 uint8_t (*Max31855_Spi_ReadByte)(void),
 void (*Max31855_Delay_1ms)(void));
bool MAX31855_Read(MAX31855_SpiDriver_Typedef * max31855_SpiDriver, MAX31855_Data_Typedef * max31855_Data);

#endif /* __MAX31855_H */
