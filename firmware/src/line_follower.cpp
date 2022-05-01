#include "globals.h"

/* LINE FOLLOW CONFIG */
#define SENSORS_PER_ARRAY 4

byte reflectance_short_pins[] = 	{6, 12, A1, 7};
byte reflectance_long_pins[]  = 	{4, 11, A0, 5};

/**
 * @brief Setup is a one time call to initialize pinout and calibrate reflectance array before use
 * 
 */
void line_follower_setup()
{
  reflectanceSensorshort.init(reflectance_short_pins, SENSORS_PER_ARRAY);
  reflectanceSensorlong.init(reflectance_long_pins, SENSORS_PER_ARRAY);

  reflectanceSensorshort.calibrate();
  reflectanceSensorlong.calibrate();
  delay(20);
}