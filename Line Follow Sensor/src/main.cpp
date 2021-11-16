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

//Line sensor connected to analog A0
//int lineS = A0; 

//Forward decleration of functions
void forward();
void stop();

void setup(){
  Serial.begin(9600);
  
  //relay_Right/LeftMotors switch for motors setup
  pinMode(relay_RightMotors, OUTPUT);
  pinMode(relay_LeftMotors, OUTPUT);

  //LineFollow
  pinMode(lineS, INPUT);
}


void loop(){

  //Reading Line Sensor
  int CENTER_LINE_SENSOR = analogRead(lineS);

  if(CENTER_LINE_SENSOR >= 770 && CENTER_LINE_SENSOR <= 850)
  {
    forward(); //Move robot forward
  }
  else
  {
    stop(); //Stop robot
  }
  Serial.println(CENTER_LINE_SENSOR); 
}

//Drive robot forward
while 
void forward()
{
  digitalWrite(relay_RightMotors, relay_RightMotors_ON);
  digitalWrite(relay_LeftMotors, relay_LeftMotors_ON);
}

//Stop robot
void stop()
{
  digitalWrite(relay_RightMotors, relay_RightMotors_OFF);
  digitalWrite(relay_LeftMotors, relay_LeftMotors_OFF);
}