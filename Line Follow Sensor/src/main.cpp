//%%%%%%%%%%%%%% Check line color function %%%%%%%%%%%%%%%%%%
//Returns value from the QRE1113
//Lower numbers mean more refleacive = white
//Our sensor range for white (770-800)
//Our sensor range for black ~1001
//More than 3000 means nothing was reflected.
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <Arduino.h>

//Defining pins
#define relay_RightMotors 53
#define relay_LeftMotors  51
#define lineS A0

//Defining relay_RightMotors ON/OFF states
#define relay_RightMotors_ON 1		// Define relay_RightMotors on pin state
#define relay_RightMotors_OFF 0		// Define relay_RightMotors off pin state

#define relay_LeftMotors_ON 1			// Define relay_LeftMotors on pin state
#define relay_LeftMotors_OFF 0		// Define relay_LeftMotors off pin state

//Defining line sensor state
int lineSensorReads;
#define Black 0
#define White 1

//Check line sensor function
void lineSensor();
//int lineS = A0; 

//motors mototion functions decleration
void ALL_forward();
void R_motorsForward();
void L_motorsForward();
void R_motorsStop();
void L_motorsStop();
void ALL_stop();

//End of white line or miss alligned function decleration
void isThisTheEnd();   //check if end of white line or miss alligned
void checkLeftSide();  //check left side of robot
void checkRightSide(); //check right side of robot 
void weMissAlligned(); //miss alligned algorithm
void weMissAlligned_LEFT();  //we miss alligned to the left side
void weMissAlligned_RIGHT(); ////we miss alligned to the right side

void setup()
{
  Serial.begin(9600);
  
  //relay_Right/LeftMotors switch for motors setup
  pinMode(relay_RightMotors, OUTPUT);
  pinMode(relay_LeftMotors, OUTPUT);

  //LineFollow
  pinMode(lineS, INPUT);
}

void loop()
{
  lineSensor(); //Check line senso

  //Determine driving on white line or verify stop
  if (lineSensorReads == White)
  {
    ALL_forward();
  }
  else
  {
    isThisTheEnd();
  }
}

/***********************************************************************
 ***********************************************************************
 *              All created function decleration below
 ***********************************************************************
***********************************************************************/
//Check line sensor
void lineSensor()
{
  //Reading Line Sensor
  int CENTER_LINE_SENSOR = analogRead(lineS);

  //determine if standing on white or black
  if(CENTER_LINE_SENSOR >= 770 && CENTER_LINE_SENSOR <= 850)
  {
    lineSensorReads = White;
  }
  else
  {
    lineSensorReads = Black;
  }
  //Serial print for testing
  Serial.println(CENTER_LINE_SENSOR); 
}

//Check if this is the end of white line
void isThisTheEnd()
{
  //first check both the left and right sides, starting with left side
  checkLeftSide();
}

//Check left side of robot
void checkLeftSide()
{
  //turn right motors for 50ms to turn left
  digitalWrite(relay_RightMotors, relay_RightMotors_ON);
  delay(50); //Delay for 50ms
  digitalWrite(relay_RightMotors, relay_RightMotors_OFF);

  lineSensor();
  if (lineSensorReads == White)
  {
    digitalWrite(relay_RightMotors, relay_RightMotors_ON);
  }
  else
  {
    void stop();
  }
}

//Drive motors forward
void ALL_forward()
{
  digitalWrite(relay_RightMotors, relay_RightMotors_ON);
  digitalWrite(relay_LeftMotors, relay_LeftMotors_ON);
  lineSensor();
}

//Stop all motors
void stop()
{
  digitalWrite(relay_RightMotors, relay_RightMotors_OFF);
  digitalWrite(relay_LeftMotors, relay_LeftMotors_OFF);
}