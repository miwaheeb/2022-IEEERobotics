#include "main.h"


/**
 * SENSOR ARRANGEMENT SETTINGS
 */
enum Clusters {North, South, East, West};
Lidar_Sensor sensor_array[CLUSTERS][CLUSTER_SENSORS]; /* List of each sensor on each cluster */
typedef Lidar_Sensor Cluster[CLUSTER_SENSORS]; /* Detection pattern object */
bool object_detected[CLUSTERS][CLUSTER_SENSORS]; /* Parallel detection status array */
typedef bool Object[CLUSTER_SENSORS]; /* Detection pattern object */

/* Actual algorithm
bool isAir[CLUSTER_SENSORS] = {false, false, false};
bool isCup[CLUSTER_SENSORS] = {true, false, true};
bool isTree[CLUSTER_SENSORS] = {false, true, false};
bool isNet[CLUSTER_SENSORS] = {true, true, true};
*/

Object air = {false, false};
bool isAir = false;
Object hand = {true, true};
bool isHand = false;

bool measureFlag = false;

void setup()  
{ 

  /* Disable polling timer */
  TIMSK2 = 0; 

	communication_setup();
  delay(100);

  /* Configure sensor polling timer to 1ms */
  TCCR2B = 1 << CS22 | 1 << CS21 | 0 << CS20;

  Serial.println("Setup complete. Beginning\n");
  delay(500);
  
  /* Enable polling timer */
  TIMSK2 = 1;
}
 
void loop() 				
{

  while(!measureFlag){}; /* Block until time to detect */
  measureFlag = false;

  void detect_objects();

  /* TODO: Confirm there isn't a better way to do this */
  /* If a cluster pattern is recognized, do X */
  if(isHand)
  {
    for(size_t i = 0; i < CLUSTERS; i++)
    {
      if(hand && *object_detected[i])
      {
        Serial.println("\tWOWOWOWOWOWOW");
        Serial.print("\tDetected on cluster ");
        Serial.print(i);
        Serial.println("\n");
      }
    }
  }
}


/**
 * @brief Uses an averaging filter with 1ms samples to poll a cluster for a reading
 * Sensor detection patterns indicate if an object is detected and what it is
 */
void detect_objects()
{
  VL53L0X_RangingMeasurementData_t measure;

  /* Calculate the running average distance for each sensor to check for objects */
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    {
        Lidar_Sensor sensor = sensor_array[i][j];
  
        /* Add the new sample to the average */
	      sensor.VL53L0X.rangingTest(&measure, false);
        sensor.weightedSample = measure.RangeMilliMeter/FILTER_LENGTH;
        sensor.distanceAverage += sensor.weightedSample;

        /* Remove the oldest sample */
        sensor.sampleIndex = (sensor.sampleIndex + 1) % FILTER_LENGTH;
        sensor.distanceAverage -= sensor.sampleWindow[sensor.sampleIndex];

        /* Store the new sample for later */
        sensor.sampleWindow[sensor.sampleIndex] = sensor.weightedSample;

        /* If it is detected within the distance parameter, catalog as true*/
        object_detected[i][j] = sensor.distanceAverage > MIN_DISTANCE && sensor.distanceAverage < MAX_DISTANCE;
    }

    /* Check each cluster pattern for a known object*/
    /* TODO: QUESTIONABLE IMPLEMENTATION */
    Object *object = &object_detected[i];
    isAir = air && *object;
    isHand = hand && *object;
      
  }
}

/* Enable all comms */
void communication_setup()
{
  /* Initialize debug serial */
  Serial.begin(BAUD_RATE);
  pinMode(INTERRUPT_PIN, INPUT);

  /* Initialize I2C */
  Wire.begin();
  Wire.setClock(I2C_CLOCK);

  Serial.println("Initializing I2C devices...");
 
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    { 
      sensor_setup(sensor_array[i][j]);
    }
  }

}

/* Setup each sensor*/
void sensor_setup(Lidar_Sensor sensor)
{
  int16_t retries;
  digitalWrite(sensor.enable, HIGH);
  delay(100);
  for (retries = 0; !sensor.VL53L0X.begin(sensor.address,false,&Wire,Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT) 
  && retries < RETRIES_MAX; retries++) 
  {
    Serial.print("Sensor on pin ");
    Serial.print(sensor.enable);
    Serial.println("was unsuccessful.");
    digitalWrite(sensor.enable, LOW);
    delay((retries) * 100); /* Add a little delay while we power cycle */
    digitalWrite(sensor.enable, HIGH);

    Serial.println("Retrying device...");
  }
    
  if (retries >= RETRIES_MAX)
  {
     Serial.println("\tCritical failure");
     delay(1000);
    
     exit(EXIT_FAILURE);
  }

  Serial.print("Sensor on pin ");
  Serial.print(sensor.enable);
  Serial.println("was successful.");
  
}


ISR(TIMER2_OVF_vect)
{
  measureFlag = true;
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