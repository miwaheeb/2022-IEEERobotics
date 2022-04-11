#include <Arduino.h>
#include <Servo.h> //red: 5V orange: 8 yellow: GND
#include <Bounce2.h>

//1 inch = (200 * 13)steps = 2600
int verDirectionPin = 2; //Direction pin
int verStepPin = 3; //Stepper pin
int verEnablePin = 7; //Motor enable pin

int horDirectionPin = 4; //Direction pin
int horStepPin = 5; //Stepper pin
int horEnablePin = 8; //Motor enable pin

#define UP_PIN 9
#define DOWN_PIN 10
#define FORWARD_PIN 11
#define BACK_PIN 12
#define OPEN_PIN 13

#define STEP_INCH 2600
#define STEP_MODIFIER 0.25F

#define ver 1
#define hor 2

float slow = 0.5;
float fast = 0.8;


Servo servo;
Bounce up_button = Bounce();
Bounce down_button = Bounce();
Bounce forward_button = Bounce();
Bounce back_button = Bounce();
Bounce open_button = Bounce();
int angle = 0;
bool isOpen = false;
bool input_guard = true;

void rotate(int stepper, int inches, float speed);

void setup()
{
  /*Sets all pin to output; the microcontroller will send them(the pins) bits, it will not expect to receive any bits from thiese pins.*/
  pinMode(verDirectionPin, OUTPUT);
  pinMode(verStepPin, OUTPUT);
  pinMode(verEnablePin, OUTPUT);

  pinMode(horDirectionPin, OUTPUT);
  pinMode(horStepPin, OUTPUT);
  pinMode(horEnablePin, OUTPUT);
 
  digitalWrite(verEnablePin, HIGH); //Disables the motor, so it can rest untill it is called uppond
  digitalWrite(horEnablePin, HIGH); //Disables the motor, so it can rest untill it is called uppond

  up_button.attach(UP_PIN,INPUT_PULLUP);
  down_button.attach(DOWN_PIN,INPUT_PULLUP);
  forward_button.attach(FORWARD_PIN,INPUT_PULLUP);
  back_button.attach(BACK_PIN,INPUT_PULLUP);
  open_button.attach(OPEN_PIN,INPUT_PULLUP);

  servo.attach(8);
  servo.write(angle);
 
  Serial.begin(9600);
}
 
void loop()
{

  forward_button.update();
  back_button.update();
  up_button.update();
  down_button.update();
  open_button.update();

  if(up_button.read() == LOW && input_guard)
  {
    Serial.println("Up");
    rotate(ver, 1, fast);
    delay(50);
    input_guard = false;
    return;
  }
  
  if(down_button.read() == LOW && input_guard)
  {
    Serial.println("Down");
    rotate(ver, -1, fast);
    delay(50);
    input_guard = false;
    return;
  }

  if(forward_button.read() == LOW && input_guard)
  {
    Serial.println("Forward");
    rotate(hor, 1, fast);
    delay(50);
    input_guard = false;
    return;
  }

  if(back_button.read() == LOW && input_guard)
  {
    Serial.println("Back");
    rotate(hor, -1, fast);
    delay(50);
    input_guard = false;
    return;
  }

  input_guard = true;
  if(open_button.fell())
  {
    Serial.println("Open");
    isOpen = !isOpen;
    servo.write(100 - 90 * !isOpen);
    delay(500);
  }

  delay(50);
}

/*The rotate function turns the stepper motor. Tt accepts two arguments: 'inches' and 'speed'
  if inches are negative, direction is opposite. Speed can be slow or fast*/
void rotate(int stepper, int inches, float speed)
{
  if (stepper == ver)
  {
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
  int steps = STEP_INCH*inches*STEP_MODIFIER;
  if (stepper == ver)
  {
    digitalWrite(verDirectionPin, direction); //Writes the direction (from our if statement above), to the EasyDriver DIR pin
  }
  else{
    digitalWrite(horDirectionPin, direction); //Writes the direction (from our if statement above), to the EasyDriver DIR pin
  }
  /*Steppin'*/
  if (stepper == ver)
  {
    for (int i = 0; i < steps; i++){
      digitalWrite(verStepPin, HIGH);
      delayMicroseconds(speed);
      digitalWrite(verStepPin, LOW);
      delayMicroseconds(speed);
    }
  }
  if (stepper == ver)
  {
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
  if (stepper == ver)
  {
    digitalWrite(verEnablePin, HIGH); //Enabling the motor, so it will move when asked to
  }
  else
  {
    digitalWrite(horEnablePin, HIGH); //Enabling the motor, so it will move when asked to
  }
}