#include <Wire.h>
#include <Filters.h>
#include <Seismometer.h>
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float elapsedTime, currentTime, previousTime;
FilterOnePole XFHigh(HIGHPASS, 1), YFHigh(HIGHPASS, 1), ZFHigh(HIGHPASS, 1);

Seismometer::Seismometer(byte pin) {
    this->pin = pin;
    this->seismicReading = 0.0;
}


void Seismometer::collecting(){
  unsigned short trimpot_val;
  float xy_vector_mag, z_vector_mag, scale_factor;
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
  previousTime = currentTime;        
  currentTime = millis();            
  elapsedTime = (currentTime - previousTime) / 1000; 
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  
  GyroX = GyroX + 0.56; 
  GyroY = GyroY - 2; 
  GyroZ = GyroZ + 0.79; 
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; 
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

  XFHigh.input(roll / 16384.0);
  YFHigh.input(pitch / 16384.0);
  ZFHigh.input(yaw / 16384.0 - 1.0);
  scale_factor = pow(2, trimpot_val / 100.0);
  xy_vector_mag = sqrt(XFHigh.output() * XFHigh.output() + YFHigh.output() * YFHigh.output()) * scale_factor;
  z_vector_mag = abs(ZFHigh.output() * scale_factor);

  this->seismicReading=map(xy_vector_mag,0.00,9.00,0,1024);

}