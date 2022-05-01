#include "globals.h"
#include "movement.h"

/**
 * @brief Debug function used to indicate when a realignment occurred and optionally, stop the motors for physical inspection. 
 * 
 */
void debug_correct_stop()
{
  Serial.println("Correcting.");
  //stopmotors();
  delay(debug_delay);
}

/**
 * @brief Setup is a one time call to initialize the Adafruit motorshield
 * 
 */
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



/**
 * @brief Test function to use reflectance arrays to stop the robot and change conditions upon reaching the end of the track
 * and if the far left and right sensors see white, assume we are at the very end and have entered the white square
 * this will need to be tested to hardcode where to stop in the white square
*/
void enter_white_box(const unsigned int speedi)
{
    M1->run(BACKWARD);
    M2->run(BACKWARD);

    M3->run(RELEASE);
    M4->run(RELEASE);

    M1->setSpeed(speedi);
    M2->setSpeed(speedi);

    M3->setSpeed(0);
    M4->setSpeed(0);

    delay(10); //enough to get fully in white square

    M1->run(RELEASE);
    M2->run(RELEASE);

    M1->setSpeed(0);
    M2->setSpeed(0);

    while(1)
    {
      //trapped in here once done
    }
}

/**
 * @brief Test function to use reflectance arrays to orient the robot and leave the white box using only the reflectance arrays
 * 
*/
void escape_white_box()
{

    //start going forward
    /*wheels(M1, M2, Adafruit_DCMotor::run, FORWARD); */
  M1->run(FORWARD);
  M2->run(FORWARD);

  M1->setSpeed(slow*M1_MOD);
  delay(50);
  M2->setSpeed(slow*M2_MOD);

  M3->setSpeed(0);
  M4->setSpeed(0);

  delay(2000); //decide how long to get out of the white square!
  /**@TODO Distance sensor to solve this? */

  Serial.print("start\n");

}


/**
 * @brief Realign the robot to the left while oriented on the short road
 * 
 * @param sensorValues Reflectance array values available if adjustments require additional control logic
 */
void turnleftshort(unsigned int sensorValues[])
{
  debug_correct_stop();

  M3->run(BACKWARD);
  M4->run(BACKWARD);

  M1->run(RELEASE);
  M2->run(RELEASE);

  M3->setSpeed(slow*M3_MOD);
  M4->setSpeed(slow * M4_MOD);

  Serial.print("left\n");
}

/**
 * @brief Realign the robot to the right while oriented on the short road
 * 
 * @param sensorValues Reflectance array values available if adjustments require additional control logic
 */
void turnrightshort(unsigned int sensorValues[])
{
  debug_correct_stop();

  M3->run(FORWARD);
  M4->run(FORWARD);

  M1->run(RELEASE);
  M2->run(RELEASE);

  M3->setSpeed(slow * M3_MOD);
  M4->setSpeed(slow * M4_MOD);

  Serial.print("right\n");
}


/**
 * @brief Realign the robot to the left while oriented on the long road
 * 
 * @param sensorValues Reflectance array values available if adjustments require additional control logic
 */
void turnleftlong(unsigned int sensorValues[])
{
  debug_correct_stop();

  M1->run(BACKWARD);
  M2->run(BACKWARD);

  M3->run(RELEASE);
  M4->run(RELEASE);

  M1->setSpeed(slow * M1_MOD);
  M2->setSpeed(slow * M2_MOD);

  Serial.print("Left long\n");
}


/**
 * @brief Realign the robot to the left while oriented on the long road when detected
 * 
 * @param sensorValues Reflectance array values available if adjustments require additional control logic
 */
void turnrightlong(unsigned int sensorValues[])
{
  debug_correct_stop();

  M1->run(FORWARD);
  M2->run(FORWARD);

  M3->run(RELEASE);
  M4->run(RELEASE);

  M1->setSpeed(slow*M1_MOD);
  M2->setSpeed(slow*M2_MOD);
  
  Serial.print("Right long\n");
}


/**
 * @brief Stop all motion for the robot
 * 
 */
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

/**
 * @brief Polling function designed to change the robot state to move along the long road
 * 
 * @param sensorValues Reflectance array values available if adjustments require additional control logic
 */
void checkturnshort(unsigned int sensorValues[])
{
  if (sensorValues[0] <= threshold)
  { 
    //does the far right sensor also detect white?
    M1->run(RELEASE);
    M2->run(RELEASE);
    M3->run(RELEASE);
    M4->run(RELEASE);

    M1->setSpeed(0);
    M2->setSpeed(0);

    delay(1000); /**@TODO Arbitrary? */

    M1->run(BACKWARD);
    M2->run(BACKWARD);

    M1->setSpeed(slow*M1_MOD);
    M2->setSpeed(slow*M2_MOD);

    Serial.print("turn\n");

    shortroad = false;

    //read new sensor values
    reflectanceSensorlong.read(sensorValues,QTR_EMITTERS_ON);


    //while the middle two sensors are black, keep updating the sensor values
    while (sensorValues[1] > threshold || sensorValues[2] > threshold)
    {
      reflectanceSensorlong.read(sensorValues,QTR_EMITTERS_ON);
      Serial.println("Reversing");
    }
    //once the middle two sensors see white, stop M1 and M2 
    //and run M3 and M4 forward on the long road section

    M1->run(RELEASE);
    M2->run(RELEASE);

    M1->setSpeed(0);
    M2->setSpeed(0);
      
    M3->run(FORWARD);
    M4->run(FORWARD);

    M3->setSpeed(speedi*M3_MOD);
    M4->setSpeed(speedi*M4_MOD);

    shortroad = false; /* This is set twice */

  }

  else
  { //no turn detected, continue straight
    M1->run(dir);
    M2->run(dir);

    M3->run(RELEASE);
    M4->run(RELEASE);

    M1->setSpeed(speedi*M1_MOD);
    M2->setSpeed(speedi*M2_MOD);

    Serial.print("forward\n");
  }
}


/**
 * @brief Polling function designed to change the robot state to move along the short road when detected
 * 
 * @param sensorValues Reflectance array values available if adjustments require additional control logic
 */
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

    M3->setSpeed(slow*M3_MOD);
    M4->setSpeed(slow*M4_MOD);

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

    M1->setSpeed(speedi*M1_MOD);
    M2->setSpeed(speedi*M2_MOD);
  }
  else
  {
    //check distance sensor here!!!!! Stop and change direction if wall detected too close
    /*tof.rangingTest(&wallmeasure, false);
    if (wallmeasure.RangeMilliMeter < walldistance){
      stopmotors();
      delay(2000);
      dir = BACKWARD;
    }*/
    
    M3->run(dir);
    M4->run(dir);

    M1->run(RELEASE);
    M2->run(RELEASE);

    M3->setSpeed(speedi*M3_MOD);
    M4->setSpeed(speedi*M4_MOD);

    Serial.print("forward\n");
  }
}