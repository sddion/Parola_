// Basic WiFi LED Matrix Display Example
// This example demonstrates the core functionality without web interface

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

// Hardware configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5
#define DATA_PIN D7
#define CS_PIN D8

// Create display object
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// WiFi credentials
const char* ssid = "YourWiFiNetwork";
const char* password = "YourWiFiPassword";

// Display settings
uint8_t brightness = 5;      // Brightness level (1-15)
uint8_t scrollSpeed = 50;    // Scroll speed (lower = faster)
String displayMessage = "Hello ESP8266!";

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Basic LED Matrix Display...");
  
  // Initialize display
  myDisplay.begin();
  myDisplay.setIntensity(brightness);
  myDisplay.displayClear();
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Display WiFi connection status
  myDisplay.displayText("WiFi Connected!", PA_CENTER, scrollSpeed, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  
  // Wait for display animation to complete
  while (!myDisplay.displayAnimate()) {
    delay(10);
  }
  
  // Start main message
  myDisplay.displayText(displayMessage.c_str(), PA_CENTER, scrollSpeed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  // Handle display animation
  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
  
  // Add your custom logic here
  // For example: read sensors, update message, etc.
  
  delay(10);
}