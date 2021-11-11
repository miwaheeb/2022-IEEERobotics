#include <Adafruit_VL53L0X.h>
#include <Wire.h>


/* LSB/g conversion from MPU6050 to meters/second^2 */
#define G2MS 16384.0
#define RETRIES_MAX 5
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13
#define BAUD_RATE 9600
#define I2C_CLOCK 400000
#define TIME_STEP 0.01