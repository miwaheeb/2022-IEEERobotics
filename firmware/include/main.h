#ifndef MAIN_H
#define MAIN_H

#include "globals.h"
#include <SPI.h>
#include <Wire.h>

const unsigned int sensRefresh = 25;  //line follow sens refresh rate
const unsigned int threshold = 1700;

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

/* Initialize data */
bool shortroad = true;
volatile bool cup = false;
volatile bool tree = false;
volatile bool net = false;

#endif /*MAIN_H */