#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "abcdxyz";  // Replace with your Wi-Fi SSID
const char* password = "abcdxyz";  // Replace with your Wi-Fi Password
const char* apiKey = "abcdxyz";  // Replace with your ThingSpeak API Key
const unsigned long channelID = 1234567;  // Replace with your ThingSpeak Channel ID

WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // Display OLED initialization message
  display.setCursor(0, 0);
  display.println("OLED Initialized");
  display.println("helo ;P");
  display.display();
  delay(1000);

  // Start Wi-Fi connection
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to Wi-Fi...");
  display.println("2.4ghz band pls");
  display.display();
  
  WiFi.begin(ssid, password);
  int wifiTimeout = 0; // Counter to prevent indefinite waiting

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20) {
    delay(500);
    display.print(".");
    display.display();
    wifiTimeout++;
  }

  display.clearDisplay();
  if (WiFi.status() == WL_CONNECTED) {
    display.setCursor(0, 0);
    display.println("Wi-Fi Connected");
    display.println(WiFi.localIP()); // Display IP address on OLED
  } else {
    display.setCursor(0, 0);
    display.println("Wi-Fi Connection Failed");
  }
  display.display();
  delay(1000);

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Display ThingSpeak initialization message
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("ThingSpeak Ready");
  display.display();
  delay(1000);
}

void loop() {
  // Check if data is available from Arduino
  if (Serial.available()) {
    // Read temperature and pulse data sent by the Arduino
    String data = Serial.readStringUntil('\n');
    int commaIndex = data.indexOf(',');
    if (commaIndex != -1) {
      String tempString = data.substring(0, commaIndex);
      String pulseString = data.substring(commaIndex + 1);

      float temperature = tempString.toFloat();
      int pulse = pulseString.toInt();

      // Display data on OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Temp: ");
      display.print(temperature);
      display.println(" C");
      display.print("Pulse: ");
      display.print(pulse);
      display.println(" BPM");
      display.display();

      // Send data to ThingSpeak
      ThingSpeak.setField(1, temperature);
      ThingSpeak.setField(2, pulse);
      int responseCode = ThingSpeak.writeFields(channelID, apiKey);

      // Show ThingSpeak upload status on OLED
      display.setCursor(0, 40);
      if (responseCode == 200) {
        display.println("Data sent to ThingSpeak");
      } else {
        display.println("ThingSpeak Error: ");
        display.println(responseCode);
      }
      display.display();
    }
  } else {
    // Show "No Data from Arduino" if nothing is received
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("No Data from Arduino");
    display.display();
  }
  delay(2000);
}
