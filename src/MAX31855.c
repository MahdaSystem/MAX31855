#include "MAX31855.h"

const float ReferenceJunctionTemperature_Step = 0.0625;
const float ThermocoupleTemperatureData_Step = 0.25;

bool MAX31855_Init(MAX31855_SpiDriver_Typedef * max31855_SpiDriver,
  void (*Max31855_CS_Low)(void),
  void (*Max31855_CS_High)(void),
  uint8_t (*Max31855_Spi_ReadByte)(void),
  void (*Max31855_Delay_1ms)(void)) {
  if(Max31855_CS_Low && Max31855_CS_High && Max31855_Spi_ReadByte && Max31855_Delay_1ms) {
    max31855_SpiDriver->Max31855_CS_Low = Max31855_CS_Low;
    max31855_SpiDriver->Max31855_CS_High = Max31855_CS_High;
    max31855_SpiDriver->Max31855_Spi_ReadByte = Max31855_Spi_ReadByte;
    max31855_SpiDriver->Max31855_Delay_1ms = Max31855_Delay_1ms;
    return true;
  }
  else {
    return false;
  }
}

bool MAX31855_Read(MAX31855_SpiDriver_Typedef * max31855_SpiDriver, MAX31855_Data_Typedef * max31855_Data) {
  if(max31855_SpiDriver) {
    max31855_SpiDriver->Max31855_CS_Low();
    max31855_SpiDriver->Max31855_Delay_1ms();
    max31855_Data->RawDataArray[0] = max31855_SpiDriver->Max31855_Spi_ReadByte();
    max31855_Data->RawDataArray[1] = max31855_SpiDriver->Max31855_Spi_ReadByte();
    max31855_Data->RawDataArray[2] = max31855_SpiDriver->Max31855_Spi_ReadByte();
    max31855_Data->RawDataArray[3] = max31855_SpiDriver->Max31855_Spi_ReadByte();
    max31855_SpiDriver->Max31855_CS_High();
    max31855_SpiDriver->Max31855_Delay_1ms();
    
    max31855_Data->RawData =
      max31855_Data->RawDataArray[3] |
      (max31855_Data->RawDataArray[2] << 8) |
      (max31855_Data->RawDataArray[1] << 16) |
      (max31855_Data->RawDataArray[0] << 24);
    
    max31855_Data->OC_Fault  = ((max31855_Data->RawData & 0x01) == 0x01) ? true : false;
    max31855_Data->SCG_Fault = ((max31855_Data->RawData & 0x02) == 0x02) ? true : false;
    max31855_Data->SCV_Fault = ((max31855_Data->RawData & 0x04) == 0x04) ? true : false;
    max31855_Data->Fault = ((max31855_Data->RawData & 0x10000) == 0x10000) ? true : false;
    max31855_Data->ReferenceJunctionTemperature = ((int16_t)(max31855_Data->RawData & 0xFFF0)) * ReferenceJunctionTemperature_Step / 16.0F;
    max31855_Data->ThermocoupleTemperatureData = ((int16_t)((max31855_Data->RawData >> 16) & 0xFFFC)) * ThermocoupleTemperatureData_Step / 4;
    
    return true;
  }
  else {
    return false;
  }
}
