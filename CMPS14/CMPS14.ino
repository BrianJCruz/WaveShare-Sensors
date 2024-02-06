#include <Wire.h>

  //Address of the CMPS14 compass on i2c
  #define _i2cAddress 0x60

  #define CONTROL_Register 0

  #define BEARING_Register 2 
  #define PITCH_Register 4 
  #define ROLL_Register 5

  #define MAGNETX_Register  6
  #define MAGNETY_Register  8
  #define MAGNETZ_Register 10

  #define ACCELEROX_Register 12
  #define ACCELEROY_Register 14
  #define ACCELEROZ_Register 16

  #define GYROX_Register 18
  #define GYROY_Register 20
  #define GYROZ_Register 22

  #define ONE_BYTE   1
  #define TWO_BYTES  2
  #define FOUR_BYTES 4
  #define SIX_BYTES  6

//---------------------------------

  byte _byteHigh;
  byte _byteLow;

  int bearing;
  int nReceived;
  signed char pitch;
  signed char roll;

  float magnetX = 0;
  float magnetY = 0;
  float magnetZ = 0;

  float accelX = 0;
  float accelY = 0;
  float accelZ = 0;
  
  float accelScale = 9.80592991914f/1000.f; // 1 m/s^2
  
  float gyroX = 0;
  float gyroY = 0;
  float gyroZ = 0;
  
  float gyroScale = 1.0f/16.f; // 1 Dps
  

void setup() {

  Serial.begin(9600);

  // Initialize i2c
  Wire.begin();
}

void loop() {
  
  // Read the Compass
  ReadCompass();

  // Read the Accelerator
  ReadAccelerator();

  // Read the Gyroscope
  ReadGyro();

  // Print data to Serial Monitor window  
  Serial.print("$CMP,");
  Serial.print(bearing);
  Serial.print(",");
  Serial.print(pitch); 
  Serial.print(",");
  Serial.print(roll);
  Serial.print(" degrees,");
  
  Serial.print("\t$ACC,");
  Serial.print(accelX,4);
  Serial.print(",");
  Serial.print(accelY,4); 
  Serial.print(",");
  Serial.print(accelZ,4);
  Serial.print(" m/s^2,");

  Serial.print("\t$GYR,");
  Serial.print(gyroX,4);
  Serial.print(",");
  Serial.print(gyroY,4); 
  Serial.print(",");
  Serial.print(gyroZ,4);
  Serial.println(" degrees/s");

}

int16_t getBearing()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(BEARING_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;

  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate full bearing
  bearing = ((_byteHigh<<8) + _byteLow) / 10;
  
  return bearing;
}

byte getPitch()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(PITCH_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}

  // Request 1 byte from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , ONE_BYTE);

  // Something has gone wrong
  if (nReceived != ONE_BYTE) return 0;

  // Read the values
  pitch = Wire.read();

  return pitch;
}

byte getRoll()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(ROLL_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 1 byte from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , ONE_BYTE);

  // Something has gone wrong
  if (nReceived != ONE_BYTE) return 0;

  // Read the values
  roll = Wire.read();

  return roll ;
}

int16_t getgyroX()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(GYROX_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate GryoX
  return ((_byteHigh<<8) + _byteLow);
}

int16_t getgyroY()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(GYROY_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate GryoY
  return ((_byteHigh<<8) + _byteLow);
}

int16_t getgyroZ()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(GYROZ_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate GryoZ
  return ((_byteHigh<<8) + _byteLow);
}

int16_t getAcceleroX()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(ACCELEROX_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;

  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate Accelerometer
  return (((int16_t)_byteHigh <<8) + (int16_t)_byteLow);
}

int16_t getAcceleroY()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(ACCELEROY_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate Accelerometer
  return (((int16_t)_byteHigh <<8) + (int16_t)_byteLow);
}

int16_t getAcceleroZ()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(ACCELEROZ_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate Accelerometer
  return (((int16_t)_byteHigh <<8) + (int16_t)_byteLow);

}

int16_t getMagnetX()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(MAGNETX_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate value
  return (((int16_t)_byteHigh <<8) + (int16_t)_byteLow);
}

int16_t getMagnetY()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(MAGNETY_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate value
  return (((int16_t)_byteHigh <<8) + (int16_t)_byteLow);
}

int16_t getMagnetZ()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(MAGNETZ_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return 0;}
  
  // Request 2 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , TWO_BYTES);

  // Something has gone wrong
  if (nReceived != TWO_BYTES) return 0;
  
  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // Calculate value
  return (((int16_t)_byteHigh <<8) + (int16_t)_byteLow);
}

void ReadCompass()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(BEARING_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){bearing = 0; pitch = 0;  roll = 0; return;}
  
  // Request 4 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , FOUR_BYTES);

  // Something has gone wrong
  if (nReceived != FOUR_BYTES) {bearing = 0; pitch = 0;  roll = 0; return;}
  
  // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  bearing = ((_byteHigh<<8) + _byteLow) / 10;

  // Read the values
  pitch = Wire.read();

  // Read the values
  roll = Wire.read();

}


void ReadAccelerator()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(ACCELEROX_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){accelX = 0; accelY = 0; accelZ = 0; return;}
  
  // Request 6 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , SIX_BYTES);

  // Something has gone wrong
  if (nReceived != SIX_BYTES) {accelX = 0; accelY = 0; accelZ = 0; return;}
  
  // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  accelX = (((int16_t)_byteHigh <<8) + (int16_t)_byteLow) * accelScale;

  // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  accelY = (((int16_t)_byteHigh <<8) + (int16_t)_byteLow) * accelScale;

  // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  accelZ = (((int16_t)_byteHigh <<8) + (int16_t)_byteLow) * accelScale;

}

void ReadGyro()
{
  // Begin communication with CMPS14
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(GYROX_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){gyroX = 0; gyroY = 0; gyroZ = 0; return;}
  
  // Request 6 bytes from CMPS14
  nReceived = Wire.requestFrom(_i2cAddress , SIX_BYTES);

  // Timed out so return
  if (nReceived != SIX_BYTES) {accelX = 0; accelY = 0; accelZ = 0; return;}
  
  // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  gyroX = (((int16_t)_byteHigh <<8) + (int16_t)_byteLow) * gyroScale;

    // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  gyroY = (((int16_t)_byteHigh <<8) + (int16_t)_byteLow) * gyroScale;

  // Read the values
  _byteHigh = Wire.read(); _byteLow = Wire.read();
  gyroZ = (((int16_t)_byteHigh <<8) + (int16_t)_byteLow) * gyroScale;

}


void changeAddress(byte i2cAddress, byte newi2cAddress)
{
  // Reset the address on the i2c network
  // Ensure that you have only this module connected on the i2c network
  // The 7 bit i2c address must end with a 0. (even numbers please)
  // For example changeAddress(0x60, 0x64)

  // Address 0x60, 1 long flash, 0 short flashes
  // Address 0x62, 1 long flash, 1 short flashes
  // Address 0x64, 1 long flash, 2 short flashes
  // Address 0x66, 1 long flash, 3 short flashes
  // Address 0x68, 1 long flash, 4 short flashes
  // Address 0x6A, 1 long flash, 5 short flashes
  // Address 0x6C, 1 long flash, 6 short flashes
  // Address 0x6E, 1 long flash, 7 short flashes

  // Begin communication
  Wire.beginTransmission(i2cAddress);
  Wire.write(CONTROL_Register);
  Wire.write(byte(0xA0));
  
  // End the transmission
  int nackCatcher = Wire.endTransmission();

  //Wait 100ms
  delay(100);

  // Begin communication
  Wire.beginTransmission(i2cAddress);
  Wire.write(CONTROL_Register);
  Wire.write(byte(0xAA));

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return;}

  //Wait 100ms
  delay(100);

  // Begin communication
  Wire.beginTransmission(i2cAddress);
  Wire.write(CONTROL_Register);
  Wire.write(byte(0xA5));

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return;}

  //Wait 100ms
  delay(100);

  // Begin communication
  Wire.beginTransmission(i2cAddress);
  Wire.write(CONTROL_Register);
  Wire.write(newi2cAddress);

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){return;}

}
