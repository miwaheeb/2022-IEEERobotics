#include "main.h"

#define TCAADDR 0x72
#define CUP_ALERT_PIN 25
#define TREE_ALERT_PIN 24
#define NET_ALERT_PIN 23
#define NOP __asm__ __volatile__ ("nop\n\t")
/* attachInterrupt(digitalPinToInterrupt(pin), ISR, mode) ISR as function, mode:LOW CHANGE RISING FALLING... using rising */

#define debug_message Serial.print

/**
 * SENSOR ARRANGEMENT SETTINGS
 */
enum Clusters {Front, Back, Left, Right};
Lidar_Sensor sensor_array[CLUSTERS][CLUSTER_SENSORS]; /* List of each sensor on each cluster */
bool object_detected[CLUSTERS][CLUSTER_SENSORS]; /* Parallel detection status array */
typedef bool Object[CLUSTER_SENSORS]; /* Detection pattern object */

/* Digital pins 2-19*/
Pin sensor_enable_pins[18] = {6,7,1,5,15,16,17,18,23,24,25,26,64,63,13,12,46,19};

/* Digital pin 22, 23, 24 */
Pin detection_alert_pins[3] = {CUP_ALERT_PIN, TREE_ALERT_PIN, NET_ALERT_PIN};

/* Object detection patterns */
Object Air = {false, false, false};
Object Cup = {true, false, true};
Object Tree = {false, true, false};
Object Net = {true, true, true};

volatile int measureFlag = 0;
bool address_found = false;

char msg[200];

void setup()  
{ 

  Serial.begin(115200);

  /* Disable polling timer */
  TIMSK2 = 0; 

	communication_setup();
  
  /* Configure sensor polling timer to 1ms */
  TCCR2B = 1 << CS22 | 0 << CS21 | 0 << CS20;

  sprintf(msg,"Setup complete. \nBeginning\n\n");
  debug_message(msg);
  delay(500);

  pinMode(CUP_ALERT_PIN, OUTPUT);
  pinMode(TREE_ALERT_PIN, OUTPUT);
  pinMode(NET_ALERT_PIN, OUTPUT);

  /* Enable polling timer */
  TIMSK2 = 1;

  sei();

}


void loop() 				
{

  if(!address_found)
  {
    byte count = 0;
    Serial.println ("I2C scanner. Scanning ...");

    for(int j = 0; j < 8; j++)
    {
      Wire.beginTransmission(TCAADDR);
      Wire.write(1<<j);
      Wire.endTransmission(true);
      
      for (byte i = 1; i < 120; i++)
      {
        Wire.beginTransmission (i);

        if (Wire.endTransmission () != 0 || i == TCAADDR)
          continue;

        sprintf(msg, "Found address: %d (0x%2X) on port %d.\n", i, i, j);
        debug_message(msg);
        count++;
        delay (1);  
      }

    }

    sprintf(msg, "\nDone. Found %d device(s).\n", count);
    debug_message(msg);

    address_found = true;
  }

  while(measureFlag){;} /* Block until time to detect */
  
  detect_objects();
  
  /* If a cluster pattern is recognized, do X */
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    bool isAir  = !((bool)memcmp(Air, &object_detected[i], sizeof(bool)*CLUSTER_SENSORS));
    bool isCup  = !((bool)memcmp(Cup, &object_detected[i], sizeof(bool)*CLUSTER_SENSORS));
    bool isTree = !((bool)memcmp(Tree,&object_detected[i], sizeof(bool)*CLUSTER_SENSORS));
    bool isNet  = !((bool)memcmp(Net, &object_detected[i], sizeof(bool)*CLUSTER_SENSORS));

    if(isAir)
    {

    }
    if(isCup)
    {
      sprintf(msg, "\tDetected cup on cluster %d\n", i);
      debug_message(msg);
      
      digitalWrite(CUP_ALERT_PIN, HIGH);
      NOP;
      NOP;
      digitalWrite(CUP_ALERT_PIN, LOW);
    }
    if(isTree)
    {
      sprintf(msg, "\tDetected tree on cluster %d\n", i);
      debug_message(msg);
      digitalWrite(TREE_ALERT_PIN, HIGH);
      NOP;
      NOP;
      digitalWrite(TREE_ALERT_PIN, LOW);
    }
    if(isNet)
    {
      sprintf(msg, "\tDetected net on cluster %d\n", i);
      debug_message(msg);
      digitalWrite(NET_ALERT_PIN, HIGH);
      NOP;
      NOP;
      digitalWrite(NET_ALERT_PIN, LOW);
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
      Lidar_Sensor *sensor = &sensor_array[i][j];

      Wire.beginTransmission(TCAADDR);
      Wire.write(1 << sensor->address);
      Wire.endTransmission(true);

      /* Add the new sample to the average */
      int valid = sensor->VL53L0X.rangingTest(&measure, false);
      sensor->weightedSample = measure.RangeMilliMeter/FILTER_LENGTH;

      //sprintf(msg, "Measurement taken at: %d on address %d was a %d measurement\n", measure.RangeMilliMeter, sensor->address, valid);
      //debug_message(msg);

      sensor->distanceAverage += sensor->weightedSample;

      /* Remove the oldest sample */
      sensor->sampleIndex = (sensor->sampleIndex + 1) % FILTER_LENGTH;
      sensor->distanceAverage -= sensor->sampleWindow[sensor->sampleIndex];

      //sprintf(msg, "Average distance %dmm on address %d\n", measure.RangeMilliMeter, sensor->address);
      //debug_message(msg);

      /* Store the new sample for later */
      sensor->sampleWindow[sensor->sampleIndex] = sensor->weightedSample;

      /* If it is detected within the distance parameter, catalog as true*/
      object_detected[i][j] = sensor->distanceAverage > MIN_DISTANCE && sensor->distanceAverage < MAX_DISTANCE;
    }
  }
    
}

/* Enable all comms */
void communication_setup()
{
  /* Initialize I2C */
  Wire.begin();
  Wire.setClock(I2C_CLOCK);
  
  debug_message("Initializing I2C devices...\n");
  size_t k = 0;
  size_t address = 1;

  /* All sensors disabled */
  debug_message("Resetting sensors...\n");
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    { 
      pinMode(sensor_enable_pins[k], OUTPUT);
    	digitalWrite(sensor_enable_pins[k], HIGH);
      sensor_array[i][j].init(sensor_enable_pins[k++], address++);
    }
  }

   /* Initialize each sensor */
  for(size_t i = 0; i < CLUSTERS; i++)
  {
    for(size_t j = 0; j < CLUSTER_SENSORS; j++)
    { 
      sprintf(msg,"Initializing sensor on pin %d\n\n", sensor_array[i][j].enable);
      debug_message(msg);
      sensor_setup(&sensor_array[i][j]);
    }
  }
}

/* Setup each sensor*/
void sensor_setup(Lidar_Sensor *sensor)
{
  int16_t retries = 0;
 
  /* Select I2C bus from expander */
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << sensor->address);
  Wire.endTransmission(true);
  
  delay(100);

  for (retries = 1; !sensor->VL53L0X.begin() && retries < RETRIES_MAX; retries++) 
  {
    sprintf(msg,"Sensor on pin %d did not initialize.\n", sensor->enable);
    debug_message(msg);
    digitalWrite(sensor->enable, LOW);
    delay((retries) * 100); /* Add a little delay while we power cycle */
    digitalWrite(sensor->enable, HIGH);
    delay((retries) * 100); /* Add a little delay while we power cycle */
    debug_message("Retrying device...\n\n");
  }
    
  if (retries >= RETRIES_MAX)
  {
    debug_message("Critical failure!\n"); 
    delay(1000);
     
    exit(EXIT_FAILURE);
  }

  sprintf(msg,"Sensor on pin %d initialized.\n\n", sensor->enable);
  debug_message(msg);
}

ISR(TIMER2_OVF_vect)
{
  measureFlag++;
  measureFlag %= MEASURE_TIME;
}