#include "globals.h"

void line_follower_setup()
{
  reflectanceSensorshort.init(reflectance_short_pins, SENSORS_PER_ARRAY);
  reflectanceSensorlong.init(reflectance_long_pins, SENSORS_PER_ARRAY);

  reflectanceSensorshort.calibrate();
  reflectanceSensorlong.calibrate();
  delay(20);
}