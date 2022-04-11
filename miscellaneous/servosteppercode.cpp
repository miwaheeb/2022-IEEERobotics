#include <Servo.h> //red: 5V orange: 8 yellow: GND

//1 inch = (200 * 13)steps = 2600
int verDirectionPin = 2; //Direction pin
int verStepPin = 3; //Stepper pin
int verEnablePin = 7; //Motor enable pin

int horDirectionPin = 4; //Direction pin
int horStepPin = 5; //Stepper pin
int horEnablePin = 8; //Motor enable pin

#define ver 1
#define hor 2

float slow = 0.5;
float fast = 0.8;


Servo servo;
int angle = 0;

 
void setup(){
  /*Sets all pin to output; the microcontroller will send them(the pins) bits, it will not expect to receive any bits from thiese pins.*/
  pinMode(verDirectionPin, OUTPUT);
  pinMode(verStepPin, OUTPUT);
  pinMode(verEnablePin, OUTPUT);

  pinMode(horDirectionPin, OUTPUT);
  pinMode(horStepPin, OUTPUT);
  pinMode(horEnablePin, OUTPUT);
 
  digitalWrite(verEnablePin, HIGH); //Disables the motor, so it can rest untill it is called uppond
  digitalWrite(horEnablePin, HIGH); //Disables the motor, so it can rest untill it is called uppond

  servo.attach(8);
  servo.write(angle);
 
  Serial.begin(9600);
}
 
void loop(){
  /*Here we are calling the rotate function to turn the stepper motor*/
  rotate(ver, 4, fast); //number of inches for first stepper positive is take it off, negative is put it on
  delay(1000);
  rotate(ver, -4, fast); //number of inches for second stepper
  delay(1000);
  servo.write(100);
  delay(3000);
  servo.write(10);
 
}
 
/*The rotate function turns the stepper motor. Tt accepts two arguments: 'inches' and 'speed'
  if inches are negative, direction is opposite. Speed can be slow or fast*/
void rotate(int stepper, int inches, float speed){
  if (stepper == ver){
    digitalWrite(verEnablePin, LOW); //Enabling the motor, so it will move when asked to
  }
  else{
    digitalWrite(horEnablePin, LOW); //Enabling the motor, so it will move when asked to
  }
 
  /*This section looks at the 'steps' argument and stores 'HIGH' in the 'direction' variable if */
  /*'steps' contains a positive number and 'LOW' if it contains a negative.*/
  int direction;
 
  if (inches > 0){
    direction = HIGH;
  }else{
    direction = LOW;
  }
 
  speed = 1/speed * 70; //Calculating speed
  inches = abs(inches); //Stores the absolute value of the content in 'steps' back into the 'steps' variable
  int steps = 2600*inches;
  if (stepper == ver){
    digitalWrite(verDirectionPin, direction); //Writes the direction (from our if statement above), to the EasyDriver DIR pin
  }
  else{
    digitalWrite(horDirectionPin, direction); //Writes the direction (from our if statement above), to the EasyDriver DIR pin
  }
  /*Steppin'*/
  if (stepper == ver){
    for (int i = 0; i < steps; i++){
      digitalWrite(verStepPin, HIGH);
      delayMicroseconds(speed);
      digitalWrite(verStepPin, LOW);
      delayMicroseconds(speed);
    }
  }
  if (stepper == ver){
    for (int i = 0; i < steps; i++){
      digitalWrite(verStepPin, HIGH);
      delayMicroseconds(speed);
      digitalWrite(verStepPin, LOW);
      delayMicroseconds(speed);
    }
  }
  else{
    for (int i = 0; i < steps; i++){
      digitalWrite(horStepPin, HIGH);
      delayMicroseconds(speed);
      digitalWrite(horStepPin, LOW);
      delayMicroseconds(speed);
    }
  }
  if (stepper == ver){
    digitalWrite(verEnablePin, HIGH); //Enabling the motor, so it will move when asked to
  }
  else{
    digitalWrite(horEnablePin, HIGH); //Enabling the motor, so it will move when asked to
  }
}