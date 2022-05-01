
#include "main.h"

/**
 * @brief One time initialization of for the primary microcontroller and components. Ends with leaving the white starting box
 * 
 */
void setup() 
{
  Serial.begin(115200);
  interrupt_setup();
  motor_shield_setup();
  line_follower_setup();
  escape_white_box();
}

/**
 * @brief One time initialization of interrupt handlers. Called from main arduino setup.
 * 
 */
void interrupt_setup()
{
    /* Initialize interrupt pins */
  pinMode(INTERRUPT_PIN_NET, INPUT); // net
  pinMode(INTERRUPT_PIN_TREE, INPUT); //tree

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_NET), netdetect, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_TREE), treedetect, RISING);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_CUP), cupdetect, RISING);

}

/**
 * @brief Control logic while the robot is operating on the short road of the track
 * 
 */
void short_section()
{
  unsigned int sensorValues[4];
  reflectanceSensorshort.read(sensorValues,QTR_EMITTERS_ON);

  if (sensorValues[1] > threshold && sensorValues[2] < threshold)
  {
    turnleftshort(sensorValues);
    return;
  }
    
  //if center right sees white
  if (sensorValues[2] > threshold && sensorValues[1] < threshold)
  {
    turnrightshort(sensorValues);
    return;
  }

  if (sensorValues[0] > threshold || sensorValues[3] > threshold)
  {
    //enter_white_box(speediShort);
    //return;
  }

  checkturnshort(sensorValues);

}

/**
 * @brief Control logic while the robot is operating on the long road of the track
 * 
 */
void long_section()
{
  unsigned int sensorValues[4];
  reflectanceSensorlong.read(sensorValues,QTR_EMITTERS_ON);

  if (sensorValues[1] > threshold && sensorValues[2] < threshold)
  {
    turnleftlong(sensorValues);
    return;
  }
    
  //if center right sees white
  if (sensorValues[2] > threshold && sensorValues[1] < threshold)
  {
    turnrightlong(sensorValues);
    return;
  }

  if (sensorValues[0] > threshold || sensorValues[3] > threshold)
  {
    //Should we ever get here?
    //enter_white_box(speediShort);
    //Engage crying protocol
    //return;
  }

  checkturnlong(sensorValues);

}


/**
 * @brief Core control loop that implements a round-robin w/ interrupts design to handle ISRs as events
 * 
 */
void loop() 
{

  static uint16_t lastSampleTime = 0;

  //currently, any of the three interrupts just stops the motors
  if (net == true)
  {
    //stopmotors();
    //delay(5000);
    Serial.print("NET\n");
  }
  else if (tree == true)
  {
    //stopmotors();
    //delay(5000);
    Serial.print("TREE\n");
  }
  // else if (cup == true)
  // {
  //   //stopmotors();
  //   Serial.print("CUP\n");
  // }

  net = false;
  tree = false;
  cup = false;

  
  //if it has been 100ms
  if ((uint16_t)(millis() - lastSampleTime) >= sensRefresh)
  {
    lastSampleTime = millis();

    //choose which sensor array to read from depending on whether we are on the short or long section of the road
    if(shortroad)
    {
      short_section();
      return;
    }
    
    long_section();
    
  }

}

/**
 * @brief ISR to enable flags when a cup is detected from the secondary microcontroller
 * 
 */
void cupdetect()
{
  cup = true;
}

/**
 * @brief ISR to enable flags when a tree is detected from the secondary microcontroller
 * 
 */
void treedetect()
{
  tree = true;
}

/**
 * @brief ISR to enable flags when a net is detected from the secondary microcontroller
 * 
 */
void netdetect()
{
  net = true;
}
