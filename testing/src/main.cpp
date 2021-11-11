#include "main.h"
#include <stdbool.h>
#include <avr/interrupt.h>

Adafruit_VL53L0X lidar1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lidar2 = Adafruit_VL53L0X();


#define LIDAR1_ENABLE 6
#define LIDAR2_ENABLE 8
#define SAMPLE_RATE 1 

/*Time in milliseconds*/
#define THRESHOLD_TIME 5
#define FILTER_LENGTH SAMPLE_RATE*THRESHOLD_TIME

void device_setup();
const uint32_t filterLength = FILTER_LENGTH;
uint16_t sampleIndex = 0,  sampleRate = 0, thresholdTime = 0;
float weightedSample = 0, distanceAverage = 0;
float sampleWindow[filterLength];
bool measureFlag = false;

void setup()  
{ 
  TIMSK2 = 0;
  //pinMode(OUTPUT, LIDAR1_ENABLE);
  //digitalWrite(LIDAR1_ENABLE, LOW);
  //pinMode(OUTPUT, LIDAR2_ENABLE);
  //digitalWrite(LIDAR2_ENABLE, LOW);
	device_setup();
  delay(100);
  TCCR2B = 1 << CS22 | 1 << CS21 | 0 << CS20;
  Serial.println("Setup complete. Beginning\n");
  delay(500);
  TIMSK2 = 1;
}
 
void loop() 				
{

  while(!measureFlag){};
  measureFlag = false;
  VL53L0X_RangingMeasurementData_t measure;
	lidar1.rangingTest(&measure, false);

	weightedSample = measure.RangeMilliMeter/filterLength;
  //lidar2.rangingTest(&measure,false);
	distanceAverage += weightedSample;
	sampleIndex = (sampleIndex + 1) % filterLength;
	distanceAverage -= sampleWindow[sampleIndex];
	sampleWindow[sampleIndex] = weightedSample;

	if(distanceAverage > 50 && distanceAverage < 80 && measure.RangeMilliMeter > 50 && measure.RangeMilliMeter < 80)
		Serial.println("\tTRIGGERED\n");
}


void device_setup()
{
	
  // initialize serial communication
  Serial.begin(BAUD_RATE);
  pinMode(INTERRUPT_PIN, INPUT);

  Wire.begin();
  Wire.setClock(I2C_CLOCK); // 400kHz I2C clock. 
  // initialize device
    //digitalWrite(LIDAR1_ENABLE, HIGH);
  // verify connection
  Serial.println("Initializing I2C devices...");
  delay(100);
  int16_t retries;
  for (retries = 0; !lidar1.begin() && retries < RETRIES_MAX; retries++) 
  {
    Serial.println("VL53L0X connection unsuccessful");
      //digitalWrite(LIDAR1_ENABLE, LOW);
    delay(1000);
      //digitalWrite(LIDAR1_ENABLE, HIGH);
    Wire.begin();
    Serial.begin(BAUD_RATE);

      // initialize device
    Serial.println("Initializing I2C devices...");
  }

    //digitalWrite(LIDAR2_ENABLE, HIGH);
    //delay(100);
    //lidar2.begin(0x31,false,&Wire,Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT);

  if (retries >= RETRIES_MAX)
    exit(EXIT_FAILURE);


  Serial.println("VL53L0X connection successful");
}




ISR(TIMER2_OVF_vect)
{
  measureFlag = true;
  //static unsigned int count = 0;
  /*if (count++ > THRESHOLD_TIME)
  {
    measureFlag = true;
    count = 0;
  }*/
}

/*

// Define which Wire objects to use, may depend on platform
// or on your configurations.
#define SENSOR1_WIRE Wire
#define SENSOR2_WIRE Wire
#if defined(WIRE_IMPLEMENT_WIRE1)
#define SENSOR3_WIRE Wire1
#define SENSOR4_WIRE Wire1
#else
#define SENSOR3_WIRE Wire
#define SENSOR4_WIRE Wire
#endif
// Setup mode for doing reads
typedef enum {
  RUN_MODE_DEFAULT = 1,
  RUN_MODE_ASYNC,
  RUN_MODE_GPIO,
  RUN_MODE_CONT
} runmode_t;

runmode_t run_mode = RUN_MODE_DEFAULT;
uint8_t show_command_list = 1;



typedef struct {
  Adafruit_VL53L0X *psensor; // pointer to object
  TwoWire *pwire;
  int id;            // id for the sensor
  int shutdown_pin;  // which pin for shutdown;
  int interrupt_pin; // which pin to use for interrupts.
  Adafruit_VL53L0X::VL53L0X_Sense_config_t
      sensor_config;     // options for how to use the sensor
  uint16_t range;        // range value used in continuous mode stuff.
  uint8_t sensor_status; // status from last ranging in continous.
} sensorList_t;

// Actual object, could probalby include in structure above61
Adafruit_VL53L0X sensor1;
Adafruit_VL53L0X sensor2;
#ifndef ARDUINO_ARCH_AVR // not enough memory on uno for 4 objects
Adafruit_VL53L0X sensor3;
Adafruit_VL53L0X sensor4;
#endif
// Setup for 4 sensors
sensorList_t sensors[] = {
#ifndef ARDUINO_ARCH_AVR // not enough memory on uno for 4 objects
    {&sensor1, &SENSOR1_WIRE, 0x30, 0, 1,
     Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE, 0, 0},
    {&sensor2, &SENSOR2_WIRE, 0x31, 2, 3,
     Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED, 0, 0},
    {&sensor3, &SENSOR3_WIRE, 0x32, 4, 5,
     Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT, 0, 0},
    {&sensor4, &SENSOR4_WIRE, 0x33, 6, 7,
     Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT, 0, 0}
#else
    // AVR sensors move to other pins
    {&sensor1, &SENSOR1_WIRE, 0x30, 6, 8,
     Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE, 0, 0},
    {&sensor2, &SENSOR2_WIRE, 0x31, 7, 9,
     Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED, 0, 0},
#endif
};

const int COUNT_SENSORS = sizeof(sensors) / sizeof(sensors[0]);

const uint16_t ALL_SENSORS_PENDING = ((1 << COUNT_SENSORS) - 1);
uint16_t sensors_pending = ALL_SENSORS_PENDING;
uint32_t sensor_last_cycle_time;





void setup2()
{
    bool found_any_sensors = false;
  // Set all shutdown pins low to shutdown sensors
  for (int i = 0; i < COUNT_SENSORS; i++)
    digitalWrite(sensors[i].shutdown_pin, LOW);
  delay(10);

  for (int i = 0; i < COUNT_SENSORS; i++) {
    // one by one enable sensors and set their ID
    digitalWrite(sensors[i].shutdown_pin, HIGH);
    delay(10); // give time to wake up.
    if (sensors[i].psensor->begin(sensors[i].id, false, sensors[i].pwire,
                                  sensors[i].sensor_config)) {
      found_any_sensors = true;
    } else {
      Serial.print(i, DEC);
      Serial.print(F(": failed to start\n"));
    }
  }
  if (!found_any_sensors) {
    Serial.println("No valid sensors found");
    while (1)
      ;
}

*/

/*if(mydelay++ == 0)
	{
		Serial.print("Reading a measurement... ");
	
		if (measure.RangeStatus != 4) // phase failures have incorrect data
		{  
			Serial.print("Distance (mm): "); Serial.println(distanceAverage);
		} 
		else 
		{
			Serial.println(" out of range ");
		}
	}*/