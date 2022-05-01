#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h> 
#include <ZumoShield.h>

/* INTERRUPT CONFIG */
#define INTERRUPT_PIN_NET 18
#define INTERRUPT_PIN_CUP 0
#define INTERRUPT_PIN_TREE 19

/* MOVEMENT PINS */
#define PIN_MOTOR_1 9 
#define PIN_MOTOR_ENABLE 8 

extern ZumoReflectanceSensorArray reflectanceSensorshort;
extern ZumoReflectanceSensorArray reflectanceSensorlong;

extern bool shortroad;

extern const unsigned int debug_delay;
extern const unsigned int threshold;

/* MOVEMENT FUNCTIONS */
void enter_white_box(const unsigned int speedi);
void escape_white_box();
void stopmotors();

//when on short road, turn or adjust left
void turnleftshort(unsigned int sensorValues[]);

//when on short road, turn or adjust left
void turnleftlong(unsigned int sensorValues[]);

//when on short road, turn or adjust right
void turnrightshort(unsigned int sensorValues[]);

//when on long road, turn or adjust right
void turnrightlong(unsigned int sensorValues[]);

void checkturnshort(unsigned int sensorValues[]);

void checkturnlong(unsigned int sensorValues[]);

#endif