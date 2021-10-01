#include <Arduino.h>
#include <MPU6050_6Axis_MotionApps612.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	#include "Wire.h"
#endif

/* LSB/g conversion from MPU6050 to meters/second^2 */
#define G2MS 16384.0
#define RETRIES_MAX 5
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
#define BAUD_RATE 115200
#define I2C_CLOCK 400000

void accel_zero_km_calibration();
void accel_setup();
void dmp_loop();
void dmp_setup();
void read_acceleration();

void accel_setup();