#include <NewPing.h>
#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define BUZZER_PIN 11
#define GPS_RX_PIN 3
#define GPS_TX_PIN 4
#define MAX_DISTANCE 200 // Maximum distance to check for obstacles (in cm)

// Thresholds for accelerometer readings
#define ACCEL_THRESHOLD 100  // Adjust this value based on testing

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
MPU6050 accel;
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN); // RX, TX for GPS

void setup() {
  Serial.begin(9600); // Start the serial communication
  gpsSerial.begin(9600); // Start GPS communication
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as output
  Wire.begin();
  accel.initialize(); // Initialize the accelerometer
}

void loop() {
  // Obstacle detection
  delay(50); // Small delay to avoid excessive readings
  int distance = sonar.ping_cm(); // Get the distance in cm

  // Check for obstacles
  if (distance > 0 && distance < 30) { // If an obstacle is within 30 cm
    digitalWrite(BUZZER_PIN, HIGH); // Sound the buzzer
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
  }

  // Read accelerometer data
  int16_t ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);
  Serial.print("Acceleration: ");
  Serial.print("X: "); Serial.print(ax);
  Serial.print(" Y: "); Serial.print(ay);
  Serial.print(" Z: "); Serial.println(az);

  // Check for uneven surface
  if (abs(ax) > ACCEL_THRESHOLD || abs(ay) > ACCEL_THRESHOLD || abs(az) > ACCEL_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH); // Sound the buzzer for uneven surface
  } else {
    // If no uneven surface detected, turn off the buzzer
    if (distance >= 30) { // Only turn off if no obstacle is detected
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  // Read GPS data
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    Serial.write(c); // Print GPS data to Serial Monitor
  }

  // Print the distance to the Serial Monitor for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100); // Small delay for stability
}