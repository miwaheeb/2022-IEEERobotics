/*****************************************************************************************************************
 *         File: servoMain 
 *       Author: Mina Waheeb
 *      Created: 02/2022
 *  Last Update: 02/16/2022
 *        Board: arduino uno
 * Control Part: Servo motor arm
 * 
 * servoMain: user control servo motor angle of rotation in degrees
 *            -contains sweep test
 *            -start up position var
 *            -target position
 *     
 * #########################################################################################################
 * |                                         In depth explanation                                          |
 * #########################################################################################################
 * 
 *****************************************************************************************************************/
#include <Arduino.h>
#include <Servo.h>

#define servoPin 11 // define servo pin
Servo myservo;      // create servo object

// some fun functions
void servo_test_sweep();
void servoAngle(int degrees);

/**************************************************************/
/**************************************************************/
// control servo rotation in degrees below (0-180)
int angle = 0; // degrees
int start_up_pos = 90; // degrees

// sweep from 0 to 180 degrees and back sys_check
// if = 0 -> dont test, if = 1 -> test 
bool sweep_check = 0;
/**************************************************************/
/**************************************************************/

// setup
void setup() 
{
  // attach servo variable to a pin
  myservo.attach(servoPin);
  myservo.write(start_up_pos);
  delay(1000);
}

// main loop
void loop() 
{
  if(sweep_check != 0)
  {
    servo_test_sweep();
    delay(1000);
  }
  else
  {
    servoAngle(angle);
  }
}

// servo target position function
void servoAngle(int degrees)
{
  myservo.write(degrees);
  delay(20);
}

// sweep test function
void servo_test_sweep()
{
  // sweep from 0 to 180 degrees:
  for (angle = 0; angle <= 180; angle++) 
  {
    myservo.write(angle);
    delay(20);
  }
  // and back from 180 to 0 degrees:
  for (angle = 180; angle >= 0; angle--)
  {
    myservo.write(angle);
    delay(25);
  }

  // set sweep_check back to 0 to end test
  sweep_check = 0;
}