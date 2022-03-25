#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h> 
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

#include <ZumoShield.h>
#include <Adafruit_MotorShield.h>

/* INTERRUPT CONFIG */
#define INTERRUPT_PIN_NET 2
#define INTERRUPT_PIN_CUP 18
#define INTERRUPT_PIN_TREE 19

/* LINE FOLLOW CONFIG */
#define SENSORS_PER_ARRAY 4

/* LINEAR SERVO CONFIG */
#define MIN_RANGE 700.0 /* Minimum range in microseconds for GS-1502 */
#define MAX_RANGE 2300.0 /* Maximum range in microseconds for GS-1502 */

#define PIN_MOTOR_1 9 /* Change for production board */
#define PIN_MOTOR_ENABLE 8 /* Change for production board */

extern ZumoReflectanceSensorArray reflectanceSensorshort;
extern ZumoReflectanceSensorArray reflectanceSensorlong;

extern byte reflectance_short_pins[SENSORS_PER_ARRAY];
extern byte reflectance_long_pins[SENSORS_PER_ARRAY];

extern Servo linear_servo;

extern char dir;
extern bool shortroad;
extern int button;
extern const int speedi;
extern const int slow;
extern const int threshold;
extern volatile bool cup;
extern volatile bool tree;
extern volatile bool net;

extern Adafruit_MotorShield AFMS;
extern Adafruit_DCMotor *M1;
extern Adafruit_DCMotor *M2;
extern Adafruit_DCMotor *M3;
extern Adafruit_DCMotor *M4;

#endif