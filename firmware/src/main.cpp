#include <Arduino.h>
#include <Servo.h>

#define MIN_RANGE 700.0
#define MAX_RANGE 2300.0

#define PIN_MOTOR_1 9
#define PIN_MOTOR_ENABLE 8

Servo myservo;

int servo_range(double percentage)
{
  return percentage / 100.0 * (MAX_RANGE-MIN_RANGE) + MIN_RANGE;
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_MOTOR_1, OUTPUT);
  pinMode(PIN_MOTOR_ENABLE, OUTPUT);

  digitalWrite(PIN_MOTOR_ENABLE, HIGH);

  myservo.attach(PIN_MOTOR_1, MIN_RANGE, MAX_RANGE);
}

void loop()
{
  myservo.writeMicroseconds(servo_range(50));
  Serial.print("50\n");
  delay(2000);

  myservo.writeMicroseconds(servo_range(25));
  Serial.print("25\n");
  delay(2000);

  myservo.writeMicroseconds(servo_range(75));
  Serial.print("75\n");
  delay(2000);

  myservo.writeMicroseconds(servo_range(0));
  Serial.print("0\n");
  delay(2000);
  
  myservo.writeMicroseconds(servo_range(100));
  Serial.print("100\n");
  delay(2000);

}




// Keyes SR1y relay blink sketch
 
// #define RELAY_ON 1			// Define relay on pin state
// #define RELAY_OFF 0			// Define relay off pin state
 
// #define Relay  53  			// Arduino pin where the relay connects
 
// void setup()  
// {
//   digitalWrite(Relay, RELAY_OFF);      // initialise the relay to off
//   pinMode(Relay, OUTPUT);   
//   delay(1000); 
// }
 
// void loop() 				// Turn the relay on and off in sequence
// {
//   digitalWrite(Relay, RELAY_ON);	// turn the relay on
//   delay(3000);               		// wait
 
//   digitalWrite(Relay, RELAY_OFF);	// turn the relay off
//   delay(3000);              		// wait
// }

