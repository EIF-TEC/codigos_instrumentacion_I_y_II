#include "HX711.h"

HX711 scale;

#define DATAPIN 6 
#define CLOCKPIN 7

void setup()
{
  Serial.begin(115200);
  delay(1000);

  scale.begin(DATAPIN, CLOCKPIN);
}


String input = "a";
String maxt = "500";
int maxn = 500;
float value = 0.0;
float units = 0.0;


void loop()
{
  value = scale.get_value(2);
  units = scale.get_units(10);
  Serial.print(value);
  Serial.print(",");
  Serial.println(units);
  delay(250);
  while(Serial.available()) input = Serial.readStringUntil('\n');
  if (input == "c") {
    Serial.println("***Calibration***");
    Serial.println("\nEmpty the scale, press a key to continue");
    while(!Serial.available());
    while(Serial.available()) Serial.read();
    scale.tare();
    Serial.print("Weight: ");
    Serial.println(scale.get_units(10));
    Serial.println("\nSet max weight");
    while(!Serial.available());
    while(Serial.available()) maxt = Serial.readStringUntil('\n');
    maxn = maxt.toInt();
    Serial.print("\nPut ");
    Serial.print(maxn);
    Serial.println(" gram in the scale, press a key to continue");
    while(!Serial.available());
    while(Serial.available()) Serial.read();
    scale.calibrate_scale(maxn, 10);
    Serial.print("Weight: ");
    Serial.println(scale.get_units(10));   
    input = "n";
  }
}
