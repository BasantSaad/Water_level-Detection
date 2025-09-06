#define BLYNK_TEMPLATE_ID "TMPL27N8qy621"
#define BLYNK_TEMPLATE_NAME "Water level sensor"
#define BLYNK_AUTH_TOKEN "QNQPKX2yW2cUkAsfuZcrT12wHn4cvlu5"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Pin definitions
#define TRIG_PIN D5
#define ECHO_PIN D6
#define BUZZER_PIN D7

#define LED_25 D1 
#define LED_50 D2  
#define LED_75 D3 
#define LED_100 D4 

const int tankHeight = 20;
const int fullThreshold = 5; 


char auth[] = "QNQPKX2yW2cUkAsfuZcrT12wHn4cvlu5"; // Replace with your Blynk Auth Token
char ssid[] = "Hm";
char pass[] = "010203302010";

// Cloud variables
float waterLevelPercentage;
float distanceMeasurement;
bool buzzerEnabled = true;

// Blynk virtual pins
#define VPIN_WATER_LEVEL V0
#define VPIN_DISTANCE V3
#define VPIN_BUZZER_SWITCH V2
#define VPIN_Led V1
BlynkTimer timer;

// Read ultrasonic sensor and update variables
void readSensor() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);


  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; 

  if (distance > tankHeight) distance = tankHeight;

  float waterLevel = tankHeight - distance;
  float percentage = (waterLevel / tankHeight) * 100.0;

  if (percentage < 0) percentage = 0;
  if (percentage > 100 | distance<= fullThreshold) percentage = 100;

  waterLevelPercentage = percentage;
  distanceMeasurement = distance;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" cm (");
  Serial.print(percentage);
  Serial.println("%)");

  // Update Blynk app
  Blynk.virtualWrite(VPIN_WATER_LEVEL, percentage);
  Blynk.virtualWrite(VPIN_DISTANCE, distance);

  // LEDs off initially
  digitalWrite(LED_25, LOW);
  digitalWrite(LED_50, LOW);
  digitalWrite(LED_75, LOW);
  digitalWrite(LED_100, LOW);

  if (percentage >= 25) digitalWrite(LED_25, HIGH);
  if (percentage >= 50) digitalWrite(LED_50, HIGH);
  if (percentage >= 75) digitalWrite(LED_75, HIGH);

  bool isFull = (percentage >= 100 || distance <= fullThreshold);
  if (isFull) {
    digitalWrite(LED_100, HIGH);
    if (buzzerEnabled) digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

BLYNK_WRITE(VPIN_BUZZER_SWITCH) {
  buzzerEnabled = param.asInt();
  if (!buzzerEnabled) digitalWrite(BUZZER_PIN, LOW);
  Serial.print("Buzzer Enabled: ");
  Serial.println(buzzerEnabled ? "Yes" : "No");
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LED_25, OUTPUT);
  pinMode(LED_50, OUTPUT);
  pinMode(LED_75, OUTPUT);
  pinMode(LED_100, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  Blynk.begin(auth, ssid, pass);

  // Run sensor reading every second
  timer.setInterval(1000L, readSensor);

  Serial.println("Water Level Monitoring System with Blynk Started");
}

void loop() {
  Blynk.run();
  timer.run();
}
