#include <Arduino.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP8266WiFi.h>

// Pin definitions
#define TRIG_PIN D5
#define ECHO_PIN D6
#define BUZZER_PIN D7

#define LED_25 D1   // 
#define LED_50 D2   
#define LED_75 D3   
#define LED_100 D4  // Red

// Tank configuration
const int tankHeight = 20; 
const int fullThreshold = 4; 

// WiFi credentials
const char SSID[] = "Hm";
const char PASS[] = "010203302010";

// Cloud variables
float waterLevelPercentage;
float distanceMeasurement;
bool buzzerEnabled = true;

// Thing ID & Secret (replace with yours)
const char THING_ID[] = "3f1dd4d9-fe44-4c2e-b18d-7d58f2387462";
const char SECRET_KEY[] = "8pTaH5HYk2uza7Fg!YfOpVB4?";

// Connection handler
WiFiConnectionHandler connectionHandler(SSID, PASS);

// Setup Cloud properties
void initProperties() {
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(waterLevelPercentage, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(distanceMeasurement, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(buzzerEnabled, READWRITE, ON_CHANGE, onBuzzerEnabledChange);
}

// Callback when buzzer state changes from the cloud
void onBuzzerEnabledChange() {
  if (!buzzerEnabled) {
    digitalWrite(BUZZER_PIN, LOW); // Turn off if disabled
  }
  Serial.print("Cloud Buzzer Toggle: ");
  Serial.println(buzzerEnabled ? "ON" : "OFF");
}

// Read ultrasonic sensor and update cloud values
void readSensor() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // cm

  // Clamp distance to tank range
  if (distance > tankHeight) distance = tankHeight;

  // Calculate level and percentage
  float level = tankHeight - distance;
  float percent = (level / tankHeight) * 100.0;

  if (percent < 0) percent = 0;
  if (percent > 100 ) percent = 100;

  // Update cloud variables
  distanceMeasurement = distance;
  waterLevelPercentage = percent;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Water Level: ");
  Serial.print(level);
  Serial.print(" cm (");
  Serial.print(percent);
  Serial.println("%)");

  // LEDs off
  digitalWrite(LED_25, LOW);
  digitalWrite(LED_50, LOW);
  digitalWrite(LED_75, LOW);
  digitalWrite(LED_100, LOW);

  // Turn on LEDs according to level
  if (percent >= 25) digitalWrite(LED_25, HIGH);
  if (percent >= 50) digitalWrite(LED_50, HIGH);
  if (percent >= 75) digitalWrite(LED_75, HIGH);

  // Tank full condition
  bool isFull = (percent >= 100 || distance <= fullThreshold);

  if (isFull) {
    digitalWrite(LED_100, HIGH);
    if (buzzerEnabled) {
      digitalWrite(BUZZER_PIN, HIGH);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Water Level Monitoring...");

  // Pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LED_25, OUTPUT);
  pinMode(LED_50, OUTPUT);
  pinMode(LED_75, OUTPUT);
  pinMode(LED_100, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  // Init cloud
  initProperties();
  ArduinoCloud.begin(connectionHandler);
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();

  Serial.println("System Ready.");
}

void loop() {
  ArduinoCloud.update();
  readSensor();
  delay(1000); // Adjust refresh rate
}
