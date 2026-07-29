// Laboratorio 06: Medidas de Propiedades Opticas
// Sensor: Fotoresistor (LDR - Light Dependent Resistor)
// Pin: A0 (analog input)

const int sensorPin = A0;  // Analog pin where LDR is connected
int sensorValue = 0;       // Variable to store sensor reading

void setup() {
  Serial.begin(9600);      // Initialize serial communication at 9600 baud
  pinMode(sensorPin, INPUT); // Configure pin as input
}

void loop() {
  // Read sensor value (range 0-1023)
  sensorValue = analogRead(sensorPin);
  
  // Print sensor value to terminal
  // Format: value,timestamp
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(millis());
  
  // Small delay to avoid saturating communication
  delay(100);
}
