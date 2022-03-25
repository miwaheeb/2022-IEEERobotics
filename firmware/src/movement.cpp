#include "globals.h"

void motor_shield_setup()
{
  while (!AFMS.begin()) 
  {         
	// create with the default frequency 1.6KHz
  	// if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    delay(2000);
  }

  Serial.println("Motor Shield found.");
}

void escape_white_box()
{

    //start going forward
    /*wheels(M1, M2, Adafruit_DCMotor::run, FORWARD); */
  M1->run(FORWARD);
  M2->run(FORWARD);

  M1->setSpeed(speedi);
  M2->setSpeed(speedi);

  M3->setSpeed(0);
  M4->setSpeed(0);

  //delay(100); //decide how long to get out of the white square!
  /**@TODO Distance sensor to solve this? */

  Serial.print("start\n");

}

//when on short road, turn or adjust left
void turnleftshort(unsigned int sensorValues[])
{
   M3->run(BACKWARD);
   M4->run(BACKWARD);

   M1->run(RELEASE);
   M2->run(RELEASE);

   M3->setSpeed(slow);
   M4->setSpeed(slow);

   Serial.print("left\n");
}

//when on short road, turn or adjust right
void turnrightshort(unsigned int sensorValues[])
{
  M3->run(FORWARD);
  M4->run(FORWARD);

  M1->run(RELEASE);
  M2->run(RELEASE);

  M3->setSpeed(slow);
  M4->setSpeed(slow);

  Serial.print("right\n");
}

//when on short road, turn or adjust left
void turnleftlong(unsigned int sensorValues[])
{
  M1->run(BACKWARD);
  M2->run(BACKWARD);

  M3->run(RELEASE);
  M4->run(RELEASE);

  M1->setSpeed(slow);
  M2->setSpeed(slow);

  Serial.print("Left long\n");
}

//when on long road, turn or adjust right
void turnrightlong(unsigned int sensorValues[])
{
  M1->run(FORWARD);
  M2->run(FORWARD);

  M3->run(RELEASE);
  M4->run(RELEASE);

  M1->setSpeed(slow);
  M2->setSpeed(slow);
  
  Serial.print("Right long\n");
}

//stop all wheels
void stopmotors()
{
  M1->run(RELEASE);
  M2->run(RELEASE);
  M3->run(RELEASE);
  M4->run(RELEASE);
  
  M3->setSpeed(0);
  M4->setSpeed(0);
  M1->setSpeed(0);
  M2->setSpeed(0);
}

void checkturnshort(unsigned int sensorValues[])
{
  if (sensorValues[0] <= threshold)
  { //does the far right sensor also detect white?
    M1->run(RELEASE);
    M2->run(RELEASE);
    M3->run(RELEASE);
    M4->run(RELEASE);

    M1->setSpeed(0);
    M2->setSpeed(0);

    delay(1000); /**@TODO Arbitrary? */

    M1->run(BACKWARD);
    M2->run(BACKWARD);

    M1->setSpeed(slow);
    M2->setSpeed(slow);

    Serial.print("turn\n");

    shortroad = false;

    //read new sensor values
    reflectanceSensorlong.read(sensorValues,QTR_EMITTERS_ON);


    //while the middle two sensors are black, keep updating the sensor values
    while (sensorValues[1] > threshold || sensorValues[2] > threshold)
    {
      reflectanceSensorlong.read(sensorValues,QTR_EMITTERS_ON);
      Serial.print(sensorValues[1]);
      Serial.print("\n");
      Serial.print(sensorValues[2]);
      Serial.print("\n");
    }
    //once the middle two sensors see white, stop M1 and M2 
    //and run M3 and M4 forward on the long road section

    M1->run(RELEASE);
    M2->run(RELEASE);

    M1->setSpeed(0);
    M2->setSpeed(0);
      
    M3->run(FORWARD);
    M4->run(FORWARD);

    M3->setSpeed(speedi);
    M4->setSpeed(speedi);

    shortroad = false; /* This is set twice */

  }

  else
  { //no turn detected, continue straight
    M1->run(dir);
    M2->run(dir);

    M3->run(RELEASE);
    M4->run(RELEASE);

    M1->setSpeed(speedi);
    M2->setSpeed(speedi);

    Serial.print("forward\n");
  }
}

void checkturnlong(unsigned int sensorValues[])
{
  if (sensorValues[0] <= threshold)
  { //does the far right sensor also detect white? 
    M1->run(RELEASE);
    M2->run(RELEASE);
    M3->run(RELEASE);
    M4->run(RELEASE);

    M1->setSpeed(0);
    M2->setSpeed(0);

    delay(1000);

    M3->run(BACKWARD);
    M4->run(BACKWARD);

    M3->setSpeed(slow);
    M4->setSpeed(slow);

    Serial.print("turn\n");

    shortroad = true;

    //read new sensor values
    reflectanceSensorshort.read(sensorValues,QTR_EMITTERS_ON);

    //while the middle two sensors are black, keep updating the sensor values
    while (sensorValues[1] > threshold && sensorValues[2] > threshold)
    {
      reflectanceSensorshort.read(sensorValues,QTR_EMITTERS_ON);
    }

    //once the middle two sensors see white, stop M1 and M2 
    //and run M1 and M2 backward on the short road section

    M3->run(RELEASE);
    M4->run(RELEASE);
    
    M3->setSpeed(0);
    M4->setSpeed(0);
      
    M1->run(dir);
    M2->run(dir);

    M1->setSpeed(speedi);
    M2->setSpeed(speedi);
  }
  else
  {
    //check distance sensor here!!!!! Stop and change direction if wall detected too close
    M3->run(dir);
    M4->run(dir);

    M1->run(RELEASE);
    M2->run(RELEASE);

    M3->setSpeed(speedi);
    M4->setSpeed(speedi);

    Serial.print("forward\n");
  }
}