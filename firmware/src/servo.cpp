/*
 * Micro Linear Servo
 * Tested code for full range on ENV: UNO
 * ENV: Atmega already setup not tested
 * Change pins on production board
 * https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1
 */

#include "globals.h"
#include <Servo.h>

/* LINEAR SERVO CONFIG */
#define MIN_RANGE 500.0 /* Minimum range in microseconds for Tower Pro SG90 */
#define MAX_RANGE 2400.0 /* Maximum range in microseconds for Tower Pro SG90 */


Servo tower_servo;

/**
 * @brief Convert percentage of stroke length to microseconds for more intuitive control (linear servo)
 * @param percentage distance from gear
 * @return percentage of stroke converted to microseconds
 *
*/
int servo_range(double percentage)
{
  return percentage / 100.0 * (MAX_RANGE-MIN_RANGE) + MIN_RANGE;
}

/**
 * @brief One time initialization of the servo component
 * 
 */
void servo_setup()
{
  pinMode(PIN_MOTOR_1, OUTPUT);
  pinMode(PIN_MOTOR_ENABLE, OUTPUT);

  digitalWrite(PIN_MOTOR_ENABLE, HIGH);

  tower_servo.attach(PIN_MOTOR_1, MIN_RANGE, MAX_RANGE);
}

