/*
 * Micro Linear Servo
 * Tested code for full range on ENV: UNO
 * ENV: Atmega already setup not tested
 * Change pins on production board
 * https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1
 */

#include "globals.h"

/**
 * @PARAM: percentage distance from gear
 * @RETURN: Percentage of stroke converted to microseconds
 *
*/
int servo_range(double percentage)
{
  return percentage / 100.0 * (MAX_RANGE-MIN_RANGE) + MIN_RANGE;
}

void servo_setup()
{
  pinMode(PIN_MOTOR_1, OUTPUT);
  pinMode(PIN_MOTOR_ENABLE, OUTPUT);

  digitalWrite(PIN_MOTOR_ENABLE, HIGH);

  tower_servo.attach(PIN_MOTOR_1, MIN_RANGE, MAX_RANGE);
}
