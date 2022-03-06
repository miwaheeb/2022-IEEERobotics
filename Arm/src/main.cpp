/*****************************************************************************************************************
 *       Author: Mina Waheeb
 *      Created: 03/06/2022
 *  Last Update: 03/06/2022
 *        Board: arduino uno
 * Control Part: Arm control
 * 
 * 
 * #########################################################################################################
 * |                                         In depth explanation                                          |
 * #########################################################################################################
 * 
 *****************************************************************************************************************/
#include <Servo.h>
#include <Arduino.h>
#include "functions.h"

/*************************************stepper motor vars and functions******************************************/
// Defining stepper pins
int smDirectionPin = 3; //Direction pin
int smStepPin = 2; //Stepper pin
int enable = 6; //enable

// some fun functions
void stepperExtend(int directionPin, int stepPin);
void stepperRetract(int directionPin, int stepPin);
/***************************************************************************************************************/
/**************************************servo motor vars and functions*******************************************/
#define servoPin 11 // define servo pin

// some fun functions
int servo_test_sweep(int angle, bool sweep_check);
void servoAngle(int degrees);

/***************************************************************************************************************/


/**************************************************************/
/**************************************************************/
//stepper_motor
// control extention/retraction distance in inches below
int extend  = 0; // inches
int retract = 0; // inches

//servo_motor
// control servo rotation in degrees below (0-180)
int angle = 0; // degrees
int start_up_pos = 90; // degrees

// sweep from 0 to 180 degrees and back sys_check
// if = 0 -> dont test, if = 1 -> test 
bool sweep_check = 0;
/**************************************************************/
/**************************************************************/


void setup() 
{
  /*-----------------stepper motor----------------*/
   /* defining all pin as output */
  pinMode(smDirectionPin, OUTPUT);
  pinMode(smStepPin, OUTPUT);
  pinMode(enable, OUTPUT);
  // initial state of enable pin low
  digitalWrite(enable, LOW);
  /*-----------------------------------------------*/
  // attach servo variable to a pin
  myservo.attach(servoPin);
  myservo.write(start_up_pos);
  delay(1000);
  /*------------------servo motor------------------*/

  Serial.begin(9600);
}

void loop() 
{
  //stepper_motor
 if (extend > 0)
  {
    stepperExtend(smDirectionPin,smStepPin);
    extend --;
  }

  if (retract > 0)
  {
    stepperRetract(smDirectionPin,smStepPin);
    retract --;
  }

  //servo_motor
    if(sweep_check != 0)
  {
    servo_test_sweep(angle,sweep_check);
    delay(1000);
  }
  else
  {
    servoAngle(angle);
  }
}