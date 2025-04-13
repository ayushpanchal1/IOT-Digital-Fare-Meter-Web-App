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
  
  // Display initial message
  display.setCursor(0, 0);
  display.println("Welcome!");
  display.setCursor(0, 30);
  display.println("Initializing..");
  display.println("Please Wait!");
  display.display();
  
  // Loading bar animation
  for (int i = 0; i <= 100; i += 5) {
    display.fillRect(0, 50, (i * SCREEN_WIDTH) / 100, 10, WHITE);  // Draws a filled rectangle as the bar
    display.display();
    delay(50);  // Adjust delay for speed
  }

  delay(500);

  // Start Wi-Fi connection
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to Wi-Fi..");
  display.setCursor(0, 40);
  display.println("Using 2.4Ghz band");
  display.display();
  
  WiFi.begin(ssid, password);
  int wifiTimeout = 0; // Counter to prevent indefinite waiting

  // Wait for Wi-Fi connection with dot animation
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
   if (Serial.available()) {
      // Read temperature and pulse data from Arduino
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

         // Display ThingSpeak status on OLED
         display.setCursor(0, 40);
         if (responseCode == 200) {
            display.println("Data sent to ThingSpeak");
         } else {
            display.print("Error: ");
            display.println(responseCode);
         }
         display.display();
      }
   } else {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("No Data from Arduino");
      display.display();
   }
   delay(15000); // 15 seconds delay to avoid ThingSpeak rate limit issues
}

