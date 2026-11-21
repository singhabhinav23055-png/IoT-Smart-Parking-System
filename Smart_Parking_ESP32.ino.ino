#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Wi-Fi Credentials (Change these to match your mobile hotspot SSID and Password)
const char* ssid = "Nothing Phone (4b)_3427";
const char* password = "iamsocool";

// Firebase Database URL
String firebaseHost = "https://smart-parking-esp32-da9e6-default-rtdb.asia-southeast1.firebasedatabase.app/Parking.json";

// Exactly Matched Pins from Your Wiring Notes
const int trigPins[4] = {5, 16, 32, 19};   // Slot 1, 2, 3, 4 TRIG
const int echoPins[4] = {18, 21, 23, 17};  // Slot 1, 2, 3, 4 ECHO

// Distance threshold in cm (Car present if distance < 20cm)
const int distanceThreshold = 20;

void setup() {
  Serial.begin(115200);

  // Initialize sensor pins
  for (int i = 0; i < 4; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  // Connect to Wi-Fi (Mobile Hotspot)
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String slotStatus[4];

    // Read all 4 sensors sequentially
    for (int i = 0; i < 4; i++) {
      long dist = readDistance(trigPins[i], echoPins[i]);
      if (dist <= distanceThreshold) {
        slotStatus[i] = "Occupied";
      } else {
        slotStatus[i] = "Free";
      }
      Serial.print("Slot "); Serial.print(i + 1);
      Serial.print(": "); Serial.print(dist); Serial.print("cm -> ");
      Serial.println(slotStatus[i]);
    }

    // Build JSON payload for Firebase
    String jsonPayload = "{";
    jsonPayload += "\"Slot1\":\"" + slotStatus[0] + "\",";
    jsonPayload += "\"Slot2\":\"" + slotStatus[1] + "\",";
    jsonPayload += "\"Slot3\":\"" + slotStatus[2] + "\",";
    jsonPayload += "\"Slot4\":\"" + slotStatus[3] + "\"";
    jsonPayload += "}";

    // Send HTTP PATCH request to Firebase Realtime Database
    HTTPClient http;
    http.begin(firebaseHost);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.PATCH(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.println("Firebase update successful: " + String(httpResponseCode));
    } else {
      Serial.println("Error updating Firebase: " + String(httpResponseCode));
    }
    
    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected. Reconnecting...");
    WiFi.reconnect();
  }

  delay(2000); // Update every 2 seconds
}
