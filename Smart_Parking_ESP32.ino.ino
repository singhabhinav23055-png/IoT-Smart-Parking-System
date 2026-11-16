
/*
  PROJECT: IoT Smart Parking System
  CONTROLLER: ESP32-S3 N16R8
  SENSORS: 4 x HC-SR04
  DATABASE: Firebase Realtime Database

  Slot status:
  Free = Parking space available
  Occupied = Vehicle detected
*/

// Parking slot status
String slot1 = "Free";
String slot2 = "Free";
String slot3 = "Free";
String slot4 = "Free";

// Sensor distance threshold (to be calibrated)
const int DISTANCE_THRESHOLD = 10;

// Setup
void setup() {
  Serial.begin(115200);

  Serial.println("Smart Parking System");
  Serial.println("ESP32-S3 Initialisation");

  // Sensor pin setup will be added later
  // LED pin setup will be added later
  // Wi-Fi setup will be added later
  // Firebase setup will be added later
}

// Main loop
void loop() {

  // 1. Read ultrasonic sensors

  // 2. Calculate distance

  // 3. Determine Free or Occupied

  // 4. Control LEDs

  // 5. Send status to Firebase

  delay(1000);
}
