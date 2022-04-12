#ifndef MAIN_H
#define MAIN_H

#include "globals.h"
#include "movement.h"

/* MOTOR CONFIG */
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(4);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);
Adafruit_DCMotor *M3 = AFMS.getMotor(3);
Adafruit_DCMotor *M4 = AFMS.getMotor(1);

//control block_1
const unsigned int speediShort = 100;  //M1 M2, short white line section motor speed
const unsigned int speediLong = 100;  //M1 M2, short white line section motor speed
const unsigned int speediLongM3= 100; //M3, long  white line section motor speed
const unsigned int speediLongM4= 100; //M4, long  white line section motor speed


const unsigned int slow   = 70;       //motor correction speed (M1 & M2)
const unsigned int slowM3 = 70;       //motor correction speed (M3)
const unsigned int slowM4 = 70;       //motor correction speed (M4)

const unsigned int sensRefresh = 25;  //line follow sens refresh rate
const unsigned int debug_delay = 50;  //debug_correct_stop delay amount in ms

const unsigned int threshold = 1700;
//const unsigned int walldistance = 150;
//control block_1

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
int dir = FORWARD;

bool shortroad = true;
volatile bool cup = false;
volatile bool tree = false;
volatile bool net = false;

bool button = false;
//int walldistance = 200;

#endif /*MAIN_H */