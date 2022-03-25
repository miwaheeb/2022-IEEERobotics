#ifndef MAIN_H
#define MAIN_H

#include "globals.h"
#include "movement.h"

/* MOTOR CONFIG */
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);
Adafruit_DCMotor *M3 = AFMS.getMotor(3);
Adafruit_DCMotor *M4 = AFMS.getMotor(4);

const int speedi = 254;
const int slow = 200;
const int threshold = 1700;

/* LINE FOLLOWER */
ZumoReflectanceSensorArray reflectanceSensorshort;
ZumoReflectanceSensorArray reflectanceSensorlong;

/* SETUP FUNCTIONS */
void motor_shield_setup();
void interrupt_setup();
void line_follower_setup();
void escape_white_box();
void linear_servo_setup();

/* ISRs */
void netdetect();
void cupdetect();
void treedetect();

/* Initialize global data */
byte reflectance_short_pins[] = 	{6, 12, A1, 7};
byte reflectance_long_pins[]  = 	{4, 11, A0, 5};
char dir = FORWARD;

bool shortroad = true;
volatile bool cup = 0;
volatile bool tree = 0;
volatile bool net = 0;

int button = 1;



#endif /*MAIN_H */