#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Adafruit_MotorShield.h>
#include <Adafruit_VL53L0X.h>

#define M1_MOD 1.1 //M4
#define M2_MOD 1 //M2
#define M3_MOD 1.05 //M3
#define M4_MOD 1 //M1

/* MOTOR CONFIG */
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(4);
Adafruit_DCMotor *M2 = AFMS.getMotor(2);
Adafruit_DCMotor *M3 = AFMS.getMotor(3);
Adafruit_DCMotor *M4 = AFMS.getMotor(1);

unsigned int walldistance = 200;
const unsigned int speedi = 100;  //M1 M2, short white line section motor speed
const unsigned int slow   = 70;       //motor correction speed (M1 & M2)
const unsigned int debug_delay = 50;  //debug_correct_stop delay amount in ms

int dir = FORWARD;

#endif //MOVEMENT_H