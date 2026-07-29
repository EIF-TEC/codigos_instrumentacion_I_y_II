#include "TimerOne.h"
bool timer_flag;

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR); // blinkLED to run every 0.15 seconds
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timer_flag){
    Serial.println("paso 1 segundo");
    timer_flag = false;
  }

}

void ISR(void)
{
  timer_flag = true;
}

