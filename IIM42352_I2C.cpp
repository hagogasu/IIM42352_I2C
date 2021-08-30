/*****************************************************************************
  IIM42352_I2C.cpp

 Copyright (c) 2021 Siou(鄭嘉修) Co.,Ltd.
 Arduino Library for the IIM-42352 G-SENSOR chip (USE I2C)
******************************************************************************/
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include <IIM42352_I2C.h>

IIM42352::IIM42352(int slave_address)
{
  _device_address = slave_address;
  _g_sens = 2048;
}

IIM42352::~IIM42352()
{
  _device_address = 0;
  _g_sens = 0;
}

// 初始化-IIM-42352 G-SENSOR初始設定
byte IIM42352::init(void)
{
	byte rc;
	Wire.beginTransmission(0x68);//指定I2C裝置位址 IIM-42352(0x68)
	  // 模式設定
	  // ACCEL_MODE => Bank 0(0x4Eh)bits 1:0
	  // 00: Turns accelerometer off (default)開機預設
	  // 01: Turns accelerometer off
	  // 10: Places accelerometer in Low Power (LP) Mode
	  // 11: Places accelerometer in Low Noise (LN) Mode
	Wire.write(0x4E);//指定將要寫入的暫存器位址
	Wire.write(0b00000011);//寫入值 11:(LN) Mode
	rc = Wire.endTransmission(true);
}

byte IIM42352::get_rawval(unsigned char *data)
{
  byte rc;
  rc = read(IIM42352_ACC_REG, data, 6);
  if (rc != 0) {
    Serial.println(F("Can't get IIM42352 accel value"));
  }
  return (rc);
}

byte IIM42352::get_val(float *data)
{
  byte rc;
  unsigned char val[6];
  signed short acc[3];

  rc = get_rawval(val);
  if (rc != 0) {
    return (rc);
  }

  acc[0] = ((signed short)val[0] << 8) | (val[1]); //X ACC // MSB 8bit 串 LSB 8bit 變成16bit
  acc[1] = ((signed short)val[2] << 8) | (val[3]); //Y ACC
  acc[2] = ((signed short)val[4] << 8) | (val[5]); //Z ACC

  // Convert LSB to g
  data[0] = (float)acc[0] / _g_sens; // ACC-X g
  data[1] = (float)acc[1] / _g_sens; // ACC-Y g
  data[2] = (float)acc[2] / _g_sens; // ACC-Z g

  return (rc);  
}

// 透過I2C寫入IIM-42352內部暫存器     暫存器位址 ,     要寫入的資料    ,   要寫幾個byte
byte IIM42352::write(unsigned char memory_address, unsigned char *data, unsigned char size)
{
  byte rc;
  Wire.beginTransmission(_device_address);	// 指定I2C裝置位址
  Wire.write(memory_address);				// 要寫入的暫存器位址
  Wire.write(data, size);					// 寫入資料 資料,要寫幾個byte
  rc = Wire.endTransmission(true);			// 傳輸完畢結束通訊
  return (rc);								// I2C通訊狀態
}

// 透過I2C讀取IIM-42352內部暫存器  暫存器位址    ,資料陣列(用來存讀回資料)  ,要讀幾個byte
byte IIM42352::read(unsigned char memory_address, unsigned char *data, int size)
{
  byte rc;
  unsigned char cnt;
  Wire.beginTransmission(_device_address);
  Wire.write(memory_address);
  rc = Wire.endTransmission(false);
  if (rc != 0) {
    return (rc);
  }
  Wire.requestFrom(_device_address, size, true);
  cnt = 0;
  while(Wire.available()) {
    data[cnt] = Wire.read();
    cnt++;
  }
  return (0);
}
