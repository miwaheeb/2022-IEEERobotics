#ifndef MOVEMENT_H
#define MOVEMENT_H


#define M1_MOD 1
#define M2_MOD 1
#define M3_MOD 1
#define M4_MOD 1


void motor_shield_setup();

void enter_white_box(const unsigned int speedi);

void escape_white_box();

//when on short road, turn or adjust left
void turnleftshort(unsigned int sensorValues[]);

//when on short road, turn or adjust left
void turnleftlong(unsigned int sensorValues[]);

//when on short road, turn or adjust right
void turnrightshort(unsigned int sensorValues[]);

//when on long road, turn or adjust right
void turnrightlong(unsigned int sensorValues[]);

void stopmotors();

void checkturnshort(unsigned int sensorValues[]);

void checkturnlong(unsigned int sensorValues[]);

#endif