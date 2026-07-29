#include <Wire.h>

#define BMP280_ADDRESS 0x77  // BMP280 I2C address. It can also be 0x77 depending on the sensor.

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // Set the sensor to normal mode, with default settings
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(0xF4); // Control register
  Wire.write(0x27); // Normal mode, pressure and temperature oversampling rate = 1
  Wire.endTransmission();
  
  // Set the config register to default
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(0xF5); // Config register
  Wire.write(0xA0); // Standby time = 1000 ms
  Wire.endTransmission();

  delay(100); // Give the sensor time to set up
}

void loop() {
  int32_t temp_raw, press_raw;

  // Read raw temperature and pressure data
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(0xF7); // Start with register 0xF7 (pressure MSB)
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 6);
  if (Wire.available() == 6) {
    press_raw = (Wire.read() << 12) | (Wire.read() << 4) | (Wire.read() >> 4);
    temp_raw = (Wire.read() << 12) | (Wire.read() << 4) | (Wire.read() >> 4);
  }
  
  // Convert the raw values to temperature and pressure
  int32_t t_fine = get_t_fine(temp_raw);
  float temperature = BMP280_compensate_T(t_fine);
  float pressure = BMP280_compensate_P(press_raw,t_fine);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  delay(2000); // Read every 2 seconds
}

// Function to calculate compensated temperature in °C
int32_t get_t_fine(int32_t temp_raw) {
  // Insert calibration data reading here and apply the formula
  // This example uses hardcoded calibration values, which you should replace with actual calibration data
  int32_t var1, var2, T;
  int32_t dig_T1 = 27504;
  int32_t dig_T2 = 26435;
  int32_t dig_T3 = -1000;

  var1 = ((((temp_raw >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  var2 = (((((temp_raw >> 4) - ((int32_t)dig_T1)) * ((temp_raw >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

  int32_t t_fine = var1 + var2;
  return t_fine;
}

float BMP280_compensate_T(int32_t t_fine) {
  float T = (t_fine * 5 + 128) >> 8;
  return T / 100.0;
}

// Function to calculate compensated pressure in hPa
float BMP280_compensate_P(int32_t press_raw, int32_t t_fine) {
  // Insert calibration data reading here and apply the formula
  // This example uses hardcoded calibration values, which you should replace with actual calibration data
  int64_t var1, var2, p;
  int32_t dig_P1 = 36477;
  int32_t dig_P2 = -10685;
  int32_t dig_P3 = 3024;
  int32_t dig_P4 = 2855;
  int32_t dig_P5 = 140;
  int32_t dig_P6 = -7;
  int32_t dig_P7 = 15500;
  int32_t dig_P8 = -14600;
  int32_t dig_P9 = 6000;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
  var2 = var2 + (((int64_t)dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

  if (var1 == 0) {
    return 0; // Avoid exception caused by division by zero
  }
  p = 1048576 - press_raw;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
  return (float)p / 25600.0;
}
