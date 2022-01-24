#include "main.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints

/*TODO https://www.techonthenet.com/c_language/standard_library_functions/string_h/memcmp.php*/
/* https://www.st.com/resource/en/application_note/dm00280486-using-multiple-vl53l0x-in-a-single-design-stmicroelectronics.pdf */
/* https://www.analog.com/media/en/technical-documentation/product-selector-card/i2cb.pdf */
/* https://www.st.com/resource/en/datasheet/vl53l0x.pdf */

#define CUP_ALERT_PIN 76
#define TREE_ALERT_PIN 77
#define NET_ALERT_PIN 78

/**
 * SENSOR ARRANGEMENT SETTINGS
 */
enum Clusters {Front, Back, Left, Right};
Lidar_Sensor sensor_array[CLUSTERS][CLUSTER_SENSORS]; /* List of each sensor on each cluster */
bool object_detected[CLUSTERS][CLUSTER_SENSORS]; /* Parallel detection status array */
typedef bool Object[CLUSTER_SENSORS]; /* Detection pattern object */

/* Digital pins 0-19*/
Pin sensor_enable_pins[20] = {2,3,6,7,1,5,15,16,17,18,23,24,25,26,64,63,13,12,46,19};
/* Digital pin 22, 23, 24 */
Pin detection_alert_pins[3] = {CUP_ALERT_PIN, TREE_ALERT_PIN, NET_ALERT_PIN};

/* Object detection patterns */
// Object Air = {false, false, false};
// Object Cup = {true, false, true};
// Object Tree = {false, true, false};
// Object Net = {true, true, true};
Object Air = {false};
Object Cup = {true};


bool measureFlag = false;

void setup()  
{ 
  debug_init();
  /* Disable polling timer */
  TIMSK2 = 0; 

  delay(100);
	communication_setup();
  

  /* Configure sensor polling timer to 1ms */
  TCCR2B = 1 << CS22 | 0 << CS21 | 0 << CS20;

  Serial.println("Setup complete. Beginning\n");
  delay(500);
  
  /* Enable polling timer */
  TIMSK2 = 1;

  sei();

}

int debug_timer = 0;
int stuck_timer = 0; 
void loop() 				
{

  //if(debug_timer++ == 999)
  //{
    //Serial.println("Alive.");
    //debug_timer = 0;
  //}
  
  while(!measureFlag){;} /* Block until time to detect */
  stuck_timer = 0;
  measureFlag = false;

  void detect_objects();
  //Serial.println("Objects detected.");
  /* If a cluster pattern is recognized, do X */
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    bool isAir = !((bool)memcmp(&Air, &object_detected[i], sizeof(bool)*CLUSTER_SENSORS));
    bool isCup = !((bool)memcmp(&Cup, &object_detected[i], sizeof(bool)*CLUSTER_SENSORS));
    //bool isTree = !((bool*)memcmp(Tree, &object_detected[i], sizeof(bool)*3));
    //bool isNet = !((bool*)memcmp(Net, &object_detected[i], sizeof(bool)*3));

    if(isCup)
    {
      Serial.println("\tWOWOWOWOWOWOW");
      Serial.print("\tDetected on cluster ");
      Serial.print(i);
      Serial.println("\n");
      digitalWrite(CUP_ALERT_PIN, HIGH);
    }
    // if(isTree)
    // {
    //   digitalWrite(TREE_ALERT_PIN, HIGH);
    // }
    // if(isNet)
    // {
    //   digitalWrite(NET_ALERT_PIN, HIGH);
    // }

  }
  //Serial.println("Closing.");
}


/**
 * @brief Uses an averaging filter with 1ms samples to poll a cluster for a reading
 * Sensor detection patterns indicate if an object is detected and what it is
 */
void detect_objects()
{
  VL53L0X_RangingMeasurementData_t measure;

  Serial.println("Here.");

  /* Calculate the running average distance for each sensor to check for objects */
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    {
      Lidar_Sensor *sensor = &sensor_array[i][j];

      /* Add the new sample to the average */
      sensor->VL53L0X.rangingTest(&measure, false);
      sensor->weightedSample = measure.RangeMilliMeter/FILTER_LENGTH;
      sensor->distanceAverage += sensor->weightedSample;

      /* Remove the oldest sample */
      sensor->sampleIndex = (sensor->sampleIndex + 1) % FILTER_LENGTH;
      sensor->distanceAverage -= sensor->sampleWindow[sensor->sampleIndex];

      /* Store the new sample for later */
      sensor->sampleWindow[sensor->sampleIndex] = sensor->weightedSample;

      /* If it is detected within the distance parameter, catalog as true*/
      object_detected[i][j] = sensor->distanceAverage > MIN_DISTANCE && sensor->distanceAverage < MAX_DISTANCE;

      if(stuck_timer == 1000)
      {
        Serial.print("RangeMilliMeter: ");
        Serial.println(measure.RangeMilliMeter);

        Serial.print("Weighted Sample: ");
        Serial.println(sensor->weightedSample);

        Serial.print("Sample Index: ");
        Serial.println(sensor->sampleIndex);

        Serial.print("Average distance: ");
        Serial.println(sensor->distanceAverage);


        stuck_timer = 0;
      }
      stuck_timer = stuck_timer+1;
    }
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
  size_t k = 0;
  size_t address = VL53L0X_I2C_ADDR + 1;

  Serial.println("Disabling sensors...");
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    { 
      address += 2;
      sensor_array[i][j].init(sensor_enable_pins[k++], address);
    }
  }

  delay(500);
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    { 
      Serial.print("Initializing sensor on pin ");
      Serial.println(sensor_array[i][j].enable);
      sensor_setup(&sensor_array[i][j]);
    }
  }
}

/* Setup each sensor*/
void sensor_setup(Lidar_Sensor *sensor)
{
  int16_t retries;
  digitalWrite(sensor->enable, HIGH);
  delay(100);
  for (retries = 1; !sensor->VL53L0X.begin(sensor->address,true,&Wire,Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT) 
  && retries < RETRIES_MAX; retries++) 
  {
    Serial.print("Sensor on pin ");
    Serial.print(sensor->enable);
    Serial.println(" was unsuccessful.");
    digitalWrite(sensor->enable, LOW);
    delay((retries) * 100); /* Add a little delay while we power cycle */
    digitalWrite(sensor->enable, HIGH);
    delay((retries) * 100); /* Add a little delay while we power cycle */
    Serial.println("Retrying device...");
  }
    
  if (retries >= RETRIES_MAX)
  {
     Serial.println("\tCritical failure");
     delay(1000);
    
     exit(EXIT_FAILURE);
  }

  Serial.print("Sensor on pin ");
  Serial.print(sensor->enable);
  Serial.println(" was successful.");
  
}


ISR(TIMER2_OVF_vect)
{
  measureFlag = true;
}



/*
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool Air[3] = {false, false, false};
bool Cup[3] = {true, false, true};
bool Tree[3] = {false, true, false};
bool Net[3] = {true, true, true};

int main()
{

    for(size_t j = 0; j < 8; j++)
    {

    
        for(size_t i = 0; i < 8; i++)
        {   
            bool mydata[8][3]; 
            mydata[j][0] = (bool)(i & 4U);
            mydata[j][1] = (bool)( i & 2U);
            mydata[j][2] = (bool)(i & 1U);

            bool isAir = !((bool)memcmp(&Air, &mydata[j], sizeof(bool)*3));
            bool isCup = !((bool)memcmp(&Cup, &mydata[j], sizeof(bool)*3));
            bool isTree = !((bool)memcmp(&Tree, &mydata[j], sizeof(bool)*3));
            bool isNet = !((bool)memcmp(&Net, &mydata[j], sizeof(bool)*3));

            if(isAir)
                printf("Air on %d,%d\n", j,i);

            if(isCup)
                printf("Cup on %d,%d\n", j,i);

            if(isTree)
                printf("Tree on %d,%d\n", j,i);

            if(isNet)
                printf("Net on %d,%d\n", j,i);        

        }
    }
}





*/

/*
Air on 0,0
Tree on 0,2
Cup on 0,5
Net on 0,7
Air on 1,0
Tree on 1,2
Cup on 1,5
Net on 1,7
Air on 2,0
Tree on 2,2
Cup on 2,5
Net on 2,7
Air on 3,0
Tree on 3,2
Cup on 3,5
Net on 3,7
Air on 4,0
Tree on 4,2
Cup on 4,5
Net on 4,7
Air on 5,0
Tree on 5,2
Cup on 5,5
Net on 5,7
Air on 6,0
Tree on 6,2
Cup on 6,5
Net on 6,7
Air on 7,0
Tree on 7,2
Cup on 7,5
Net on 7,7
*/









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