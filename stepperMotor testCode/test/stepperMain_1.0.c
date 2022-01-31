/*****************************************************************************************************************
 *         File: stepperMain_1.0.c 
 *       Author: Mina Waheeb
 *      Created: 11/11/2021
 *  Last Update: 1/31/2022
 *        Board: arduino uno
 * Control Part: Stepper motor arm
 * 
 * v1.0: equal part of unwinding and rewinding
 * 
 * #########################################################################################################
 * |                                         In depth explanation                                          |
 * #########################################################################################################
 * 
 *****************************************************************************************************************/
#include <Arduino.h>

// Defining stepper pins
int smDirectionPin = 3; //Direction pin
int smStepPin = 2; //Stepper pin
int enable = 6; //enable
 
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
  //wind direction (clock wise aka righty tighty)
  digitalWrite(smDirectionPin, HIGH); //Writes the direction to the EasyDriver DIR pin. (HIGH is clockwise).
  /*Slowly turns the motor 1600 steps*/
  for (int i = 0; i < 1600; i++){//1600 is the duration that controlls the active stepper time
    digitalWrite(smStepPin, HIGH);
    delayMicroseconds(700);x//700 is the speed of the stepper, lowVal = fast; highVal=slow
    digitalWrite(smStepPin, LOW);
    delayMicroseconds(700);
  }
 
  delay(1000); //Pauses for a second (the motor does not need to pause between switching direction, so you can safely remove this)
 
 //unwind direction (counter clock wise aka lefty lossy)
  digitalWrite(smDirectionPin, LOW); //Writes the direction to the EasyDriver DIR pin. (LOW is counter clockwise).
  /*Turns the motor fast 1600 steps*/
  for (int i = 0; i < 1600; i++){ //1600 is the duration that controlls the active stepper time
    digitalWrite(smStepPin, HIGH);
    delayMicroseconds(70); //70 is the speed of the stepper, lowVal = fast; highVal=slow
    digitalWrite(smStepPin, LOW);
    delayMicroseconds(70);
  }
 
  delay(1000);
}