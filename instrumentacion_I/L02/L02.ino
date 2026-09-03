float V = 0;
float Vin = 5.0;
float R = 0;
float T1 = 0;
int delayTime = 100; // 10 lecturas por segundo

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
  V = (analogRead(A0) / 1023.0) * 5;
  R = calc_R(V, Vin, vR0);
  T1 = calc_T1(R, vRR, vTR, vZero, vBETA);
  Serial.print(R);
  Serial.print(",");
  Serial.println(T1);
  delay(delayTime);
}
