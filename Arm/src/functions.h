Servo myservo;      // create servo object



/************************************stepper motor control function group*************************************/
void stepperExtend(int directionPin, int stepPin)
{
  digitalWrite(directionPin, LOW); //Writes the direction to the EasyDriver DIR pin. (LOW is counter clockwise).
  for (int i = 0; i < 5100; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700); //70 is the speed of the stepper, lowVal = fast; highVal=slow
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);
  }
}

void stepperRetract(int directionPin, int stepPin)
{
  digitalWrite(directionPin, HIGH); //Writes the direction to the EasyDriver DIR pin. (LOW is counter clockwise).
  for (int i = 0; i < 5100; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700); //70 is the speed of the stepper, lowVal = fast; highVal=slow
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);
  }
}
/**************************************************************************************************************/

/**************************************servo motor control function group**************************************/
// servo target position function
void servoAngle(int degrees)
{
  myservo.write(degrees);
  delay(20);
}

// sweep test function
int servo_test_sweep(int angle, bool sweep_check)
{
  // sweep from 0 to 180 degrees:
  for (angle = 0; angle <= 180; angle++) 
  {
    myservo.write(angle);
    delay(20);
  }
  // and back from 180 to 0 degrees:
  for (angle = 180; angle >= 0; angle--)
  {
    myservo.write(angle);
    delay(25);
  }

  // set sweep_check back to 0 to end test
  sweep_check = 0;
  return sweep_check;
}
/**************************************************************************************************************/