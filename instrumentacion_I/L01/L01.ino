#include <Wire.h>
#include <INA219.h>
INA219 ina219 (0x40);

//Instrucciones: Una vez subido, correr el monitor serial y presionar Enter para iniciar la medición, en caso de que no inicie automáticamente.
// Usar libreria INA219 (programada por Rob Tillart), de la lista de librerías.

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  if (!ina219.begin()) {
    Serial.println("INA219 not found");
    while (1){ delay(10); }}
  Serial.println("INA219 initialized");
  //Here we can set max current shunt value to calibrate the Calibration Register (8.5.1)
  ina219.setMaxCurrentShunt(1, 0.1);    //  adjust if needed
  Serial.print("Shunt value:\t");
  Serial.println(ina219.getShunt(), 4);
  Serial.print("Max current:\t");
  Serial.println(ina219.getMaxCurrent(), 4);
  //Set gain based on desired range 
  ina219.setGain(1);
  Serial.print("Gain:\t");
  Serial.println(ina219.getGain());
  //Set bus voltage range
  Serial.print("Bus voltage range:\t");
  ina219.setBusVoltageRange(16);
  Serial.println(ina219.getBusVoltageRange());

  Serial.print("Calibrated?:\t");
  Serial.println(ina219.isCalibrated());
  Serial.print("CLSB:\t");
  Serial.println(ina219.getCurrentLSB());
  Serial.println("press any key");
  while (!Serial.available()) {
  }
}

float curre = 0;
float volta = 0;
float power = 0;
float shunt = 0;
int counter = 0;
// Here we set the oversampling
int samples = 100;
int delayTime = 1000/samples;

void loop() {
  counter++;
  curre += ina219.getCurrent_mA();
  volta += ina219.getBusVoltage();
  power += ina219.getPower_mW();
  shunt += ina219.getShuntVoltage_mV();
  if (counter >= samples) {
    Serial.print(curre/samples);
    Serial.print(",");
    Serial.print(volta/samples);
    Serial.print(",");
    Serial.print(power/samples);
    Serial.print(",");
    Serial.println(shunt/samples);
    counter = 0;
    curre = 0;
    volta = 0;
    power = 0;
    shunt = 0;
  }
  delay(delayTime);
}
