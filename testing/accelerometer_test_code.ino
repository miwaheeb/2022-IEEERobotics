/*
   MPU-6050 Test

   This simple program reads and prints to the Serial Monitor window
   the raw X/Y/Z values for the accelerometer and the gyro
   It also calculates the pitch and roll values as well as the temperature
   in F and C.
    
   Connect VDD to 5V and GND to ground on the MCU
   Connect SCL to SCL on MCU and SDA to SDA on MCU

  Note that the correction values can be used to put in an offset to adjust the
  values toward 0 or in the case of the temperature to adjust it to match a
  reference temperature measurement device.
*/

#include<Wire.h>
#include <math.h>
#define MSGSIZE 80
#define MSGPRINTS 10
const int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double pitch,roll;
char buffer[MSGSIZE];

//===============================================================================
//  Initialization
//===============================================================================
void setup(){
Wire.begin();
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
}
//===============================================================================
//  Main
//===============================================================================
void loop(){
Wire.beginTransmission(MPU);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU,14,true);

int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;
int temp,toff;
int count = 0;
double t,tx,tf;

//Acceleration data correction
AcXoff = 1504;
AcYoff = -94;
AcZoff = -14849;

//Temperature correction
toff = -8099;

//Gyro correction
GyXoff = 181;
GyYoff = -166;
GyZoff = -266;

//read accel data and apply correction
AcX=(Wire.read()<<8|Wire.read()) + AcXoff;
AcY=(Wire.read()<<8|Wire.read()) + AcYoff;
AcZ=(Wire.read()<<8|Wire.read()) + AcZoff;

//read temperature data & apply correction
temp=(Wire.read()<<8|Wire.read()) + toff;

//read gyro data & apply correction
GyX=(Wire.read()<<8|Wire.read()) + GyXoff;
GyY=(Wire.read()<<8|Wire.read()) + GyYoff;
GyZ=(Wire.read()<<8|Wire.read()) + GyZoff;

// Calculate and convert temperature
tx=temp;
t = tx/340 + 36.53;     // Formula from data sheet
tf = (t * 9/5) + 32;    // Standard C to F conversion

getAngle(AcX,AcY,AcZ);

sprintf(buffer,"Angle: \n\tPitch: %d\n\tRoll: %d\nAccelerometer\n\tX: %d\n\tY: %d\n\tZ: %d\n",round(pitch), round(roll), AcX, AcY, AcZ);
Serial.println(buffer);

delay(10);
}

//===============================================================================
//  GetAngle - Converts accleration data to yaw,pitch, and roll (in that order)
//===============================================================================
void getAngle(int Vx,int Vy,int Vz) {
double x = Vx;
double y = Vy;
double z = Vz;
pitch = atan(x/sqrt((y*y) + (z*z)));
roll = atan(y/sqrt((x*x) + (z*z)));
//convert radians into degrees
pitch = pitch * (180.0/3.14);
roll = roll * (180.0/3.14) ;
}
