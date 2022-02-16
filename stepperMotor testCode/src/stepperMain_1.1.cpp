/*****************************************************************************************************************
 *         File: stepperMain_1.1.c 
 *       Author: Mina Waheeb
 *      Created: 11/11/2021
 *  Last Update: 1/31/2022
 *        Board: arduino uno
 * Control Part: Stepper motor arm
 * 
 * v1.1: delaying for 5100ms ~= 1-inch
 *       user control the extention/retraction distance of stepper motor in units of inches
 * 
 * #########################################################################################################
 * |                                         In depth explanation                                          |
 * #########################################################################################################
 * 
 *****************************************************************************************************************/
#include <Arduino.h>
//#include <main.h> // functions lib

// Defining stepper pins
int smDirectionPin = 3; //Direction pin
int smStepPin = 2; //Stepper pin
int enable = 6; //enable

// some fun functions
void stepperExtend();
void stepperRetract();

/**************************************************************/
/**************************************************************/
// control extention/retraction distance in inches below
int extend  = 0; // inches
int retract = 2; // inches
/**************************************************************/
/**************************************************************/

void setup(){
  /* defining all pin as output */
  pinMode(smDirectionPin, OUTPUT);
  pinMode(smStepPin, OUTPUT);
  pinMode(enable, OUTPUT);
  
  // initial state of enable pin low
  digitalWrite(enable, LOW);

  Serial.begin(9600);
 
}
 
void loop(){
  if (extend > 0)
  {
    stepperExtend();
    extend --;
  }

  if (retract > 0)
  {
    stepperRetract();
    retract --;
  }
}


void stepperExtend()
{
  digitalWrite(smDirectionPin, LOW); //Writes the direction to the EasyDriver DIR pin. (LOW is counter clockwise).
  for (int i = 0; i < 5100; i++)
  {
    digitalWrite(smStepPin, HIGH);
    delayMicroseconds(700); //70 is the speed of the stepper, lowVal = fast; highVal=slow
    digitalWrite(smStepPin, LOW);
    delayMicroseconds(700);
  }
}

void stepperRetract()
{
  digitalWrite(smDirectionPin, HIGH); //Writes the direction to the EasyDriver DIR pin. (LOW is counter clockwise).
  for (int i = 0; i < 5100; i++)
  {
    digitalWrite(smStepPin, HIGH);
    delayMicroseconds(700); //70 is the speed of the stepper, lowVal = fast; highVal=slow
    digitalWrite(smStepPin, LOW);
    delayMicroseconds(700);
  }
}