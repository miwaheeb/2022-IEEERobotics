#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <stdbool.h>
#include <avr/interrupt.h>


/******************
 * SENSOR SETTINGS
 ******************/

/* Trigger Settings */
#define THRESHOLD_TIME 5 /* in milliseconds */
#define FILTER_LENGTH THRESHOLD_TIME
#define MIN_DISTANCE 50 /* in millimeters */
#define MAX_DISTANCE 80 /* in millimeters */

/* Group settings */
#define CLUSTERS 1
#define CLUSTER_SENSORS 1


/******************
 * ATMEGA SETTINGS
 ******************/
#define RETRIES_MAX 5
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define BAUD_RATE 9600
#define I2C_CLOCK 100000

typedef size_t Pin;

class Lidar_Sensor
{
  public:
    Adafruit_VL53L0X VL53L0X;
	  uint8_t address = 0;
    Pin enable = 0;
    uint16_t sampleIndex = 0,  thresholdTime = 0;
    float weightedSample = 0, distanceAverage = 0;
    float sampleWindow[FILTER_LENGTH];
	  bool isTriggered = false;
	  Lidar_Sensor(){}
    void init(Pin enable_pin, size_t i2c_address)
    {
		  address = i2c_address;
		  enable = enable_pin;
    	pinMode(enable, OUTPUT);
    	digitalWrite(enable, LOW);
    }

};

void communication_setup();
void sensor_setup(Lidar_Sensor *sensor);
void detect_objects();
