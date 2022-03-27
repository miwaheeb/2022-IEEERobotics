
#include "main.h"

/* Readability experiment, likely to fail */
void wheels(Adafruit_DCMotor *wheel_a, Adafruit_DCMotor *wheel_b, void(Adafruit_DCMotor::*command)(uint8_t), uint8_t parameter)
{
  (wheel_a->*command)(parameter);
  (wheel_b->*command)(parameter);
}

void setup() 
{
  Serial.begin(115200);
  interrupt_setup();
  motor_shield_setup();
  line_follower_setup();
  escape_white_box();
}

void interrupt_setup()
{
    /* Initialize interrupt pins */
  pinMode(INTERRUPT_PIN_NET, INPUT); // net
  pinMode(INTERRUPT_PIN_TREE, INPUT); //tree
  //pinMode(INTERRUPT_PIN_CUP, INPUT); //cup

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_NET), netdetect, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_TREE), treedetect, RISING);
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_CUP), cupdetect, RISING);

}

void loop() 
{
  unsigned int sensorValues[4];
  static uint16_t lastSampleTime = 0;

  //currently, any of the three interrupts just stops the motors
  if (net == true)
  {
    //stopmotors();
    Serial.print("NET\n");
  }
  else if (tree == true)
  {
    stopmotors();
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
    if (!shortroad)
    {
      reflectanceSensorlong.read(sensorValues,QTR_EMITTERS_ON);
    }
    else
    {
      reflectanceSensorshort.read(sensorValues,QTR_EMITTERS_ON);
    }
    
  }

  /**@TODO Function + Guard clause needed */
  if (sensorValues[1] <= threshold)
   { 
     //if center right sees white
    if (sensorValues[2] <= threshold)
    { 
      //and if center left sees white
      if (shortroad)
      { 
        //and if we are on the short section 
        if (sensorValues[0] <= threshold && sensorValues[3] <= threshold)
        { 
          /*@TODO: Enter white square function*/
          
          //and if the far left and right sensors see white, assume we are at the very end and have entered the white square
          //this will need to be tested to hardcode where to stop in the white square
          // M1->run(BACKWARD);
          // M2->run(BACKWARD);

          // M3->run(RELEASE);
          // M4->run(RELEASE);

          // M1->setSpeed(speedi);
          // M2->setSpeed(speedi);

          // M3->setSpeed(0);
          // M4->setSpeed(0);

          // delay(10); //enough to get fully in white square

          // M1->run(RELEASE);
          // M2->run(RELEASE);

          // M1->setSpeed(0);
          // M2->setSpeed(0);

          // while(1)
          // {
          //   //trapped in here once done
          // }
        }
        //if not all white, just center two, check for a turn/align
        else
        {
          checkturnshort(sensorValues);
        }
      }
      //if not on short road, it can't be all white (unless things be real messed up)
      
      else
      {
        //check for a turn/align 
        checkturnlong(sensorValues);
      }
    }
    //center right sees white, but left doesnt so turn right to get aligned
    else
    {
      if (!shortroad)
      {
        turnrightlong(sensorValues); //name?
      }
      else
      {
        turnrightshort(sensorValues);
      }
    }
   }

   //center right doesnt see white, check if left center sees right, and if so, align to the left
  /**@TODO Function + Guard clause needed */
  else if (sensorValues[2] <= threshold)
  {
    if (!shortroad)
    {
      turnleftlong(sensorValues); //name?
    }
    else
    {
      turnleftshort(sensorValues);
    }
  }   
}

void cupdetect()
{
  cup = true;
}

void treedetect()
{
  tree = true;
}

void netdetect()
{
  net = true;
}
