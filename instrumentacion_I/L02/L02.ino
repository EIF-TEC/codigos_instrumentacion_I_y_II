float V = 0;
float Vin = 0;
float vR = 0;
float R = 0;
float T1 = 0;
float T2 = 0;
int counter = 0;
int samples = 100;
int delayTime = 1000/samples;

#define vR0 4587.0
#define vBETA 4300
#define vTR 298.15
#define vRR 10000
#define vZero 273.15

float calc_R (float V, float Vin, float R0) {
  float R = (Vin / V) - 1; 
  R = R0 / R;
  return R;
}

float calc_T1 (float R, int RR, float TR, float Zero, int BETA) {
  float T1 = BETA + TR * log(R/RR);
  T1 = (BETA * TR) / T1;
  T1 = T1 - Zero;
  return T1;
}

void setup() {
  // initialize serial communication at 9600 bits per second 
  Serial.begin(9600) ;

}

void loop() {
  counter++;
  V = (analogRead(A1) / 1023.0) * 5;
  Vin = (analogRead(A0) / 1023.0) * 5;
  vR = calc_R(V, Vin, vR0);
  R = R + vR;
  T1 += calc_T1(vR, vRR, vTR, vZero, vBETA);
  if (counter >= samples) {
    Serial.print(R/samples);
    Serial.print(",");
    Serial.print(T1/samples);
    Serial.print(",");
    Serial.println(T2/samples);
    counter = 0;
    R = 0;
    T1 = 0;
    T2 = 0;
  }
  delay(delayTime);
}