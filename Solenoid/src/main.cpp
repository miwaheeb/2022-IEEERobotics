/*
 * Created by ArduinoGetStarted.com

 modified by mina waheeb 3/6/2022
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-solenoid-lock
 */

#include <Arduino.h>

// constants won't change
const int RELAY_2_PIN = A5;  // Sol_2 control pin
const int RELAY_1_PIN = A4;  // Sol_1 control pin


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 3 as an output.
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_1_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RELAY_1_PIN, HIGH); // uhlock the door
  digitalWrite(RELAY_2_PIN, LOW);
  delay(5000);
  digitalWrite(RELAY_1_PIN, LOW);  // lock the door
  digitalWrite(RELAY_2_PIN, HIGH);
  delay(5000);
}
