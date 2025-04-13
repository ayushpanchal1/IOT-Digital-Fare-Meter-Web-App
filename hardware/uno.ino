#include <SoftwareSerial.h>

const int lm35Pin = A0; // LM35 temperature sensor connected to A0
const int pulsePin = A1; // Pulse sensor connected to A1
SoftwareSerial espSerial(10, 11); // RX, TX for ESP8266 communication

void setup() {
  Serial.begin(115200); // Initialize Serial Monitor for debugging
  espSerial.begin(115200); // Initialize serial connection to ESP8266
}

void loop() {
  // Read temperature from LM35
  int lm35Reading = analogRead(lm35Pin); // Temp sensor reading
  float temperature = (lm35Reading * 5.0 * 100.0) / 1024.0; // Convert temp to Celsius

  // Read pulse from pulse sensor
  int pulse = analogRead(pulsePin); // Pulse sensor reading
  pulse = map(pulse, 0, 1023, 0, 100);

  // Send data to ESP8266
  Serial.print(temperature);
  Serial.print(",");
  Serial.println(pulse);
  espSerial.print(temperature);
  espSerial.print(",");
  espSerial.println(pulse);

  delay(2000); // Send data every 2 seconds to ESP8266
}
