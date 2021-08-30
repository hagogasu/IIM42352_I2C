// =============================================================
// IIM42352_I2C.h 
// Arduino Library for the IIM-42352 G-SENSOR chip (USE I2C)
// (c) Siou (鄭嘉修)
// 2021.08.30
// =============================================================

#ifndef IIM42352_I2C_h
#define IIM42352_I2C_h

#define IIM42352_DEVICE_ADDRESS_68   (0x68)    // (pin1 AP_AD0=0) 7bit Address = 0x68
#define IIM42352_DEVICE_ADDRESS_69   (0x69)    // (pin1 AP_AD0=1) 7bit Address = 0x69

#define IIM42352_ACC_REG             (0x1F)    // IIM42352 暫存器位址 XYZ ACC 數據起始位址

// 請參考範例 TES2.ino
#define MACRO_IIM42352_ACC_READ(ADDR,REG,requestSize,val,cnt) \
        Wire.beginTransmission(ADDR);\
        Wire.write(REG);\
        if (Wire.endTransmission(false) != 0) {\
          Serial.print("I2C error");\
        }\
        Wire.requestFrom(ADDR,requestSize,true);\
        cnt = 0;\
        while(Wire.available()) {\
          val[cnt] = Wire.read();\
          cnt++;\
        }\
        

#include <Arduino.h>
#include <Wire.h>

class IIM42352
{
  public:
    IIM42352(int slave_address);
    ~IIM42352();
    byte init(void);
    byte get_rawval(unsigned char *data);
    byte get_val(float *data);
    byte write(unsigned char memory_address, unsigned char *data, unsigned char size);
    byte read(unsigned char memory_address, unsigned char *data, int size);
  private:
    int _device_address;
    unsigned short _g_sens;
};

#endif
