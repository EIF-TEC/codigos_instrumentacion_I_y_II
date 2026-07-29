/*
 * Nivel
*/

const int trigPin = 9;
const int echoPin = 10;

float duration, distance, volume;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  // distance = 7.96 - distance;
  // volume = 32.05 * distance + 29.492;
  Serial.print("H: ");
  Serial.println(distance);
  // Serial.print(" V:");
  // Serial.println(volume);
  delay(100);
}