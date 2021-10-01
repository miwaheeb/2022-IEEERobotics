#include "main.h"
MPU6050 accel;

void setup()  
{ 
  //accel_setup();
  dmp_setup();
}
 
void loop() 				// Turn the relay on and off in sequence
{
  //read_acceleration();
  //delay(500);
  //dmp_loop();
}


void accel_setup()
{
  Wire.begin();
  Wire.setClock(I2C_CLOCK); // 400kHz I2C clock. Comment this line if having compilation difficulties

  // initialize serial communication
  Serial.begin(BAUD_RATE);
  pinMode(INTERRUPT_PIN, INPUT);

  Wire.begin();
  Wire.setClock(I2C_CLOCK); // 400kHz I2C clock. Comment this line if having compilation difficulties


    Serial.begin(BAUD_RATE);
    // initialize device
    Serial.println("Initializing I2C devices...");
    accel.initialize();

    // verify connection
    Serial.println("Testing device connections...");

    int16_t retries;
    for (retries = 0; !accel.testConnection() && retries < RETRIES_MAX; retries++) 
    {
      Serial.println("MPU6050 connection unsuccessful");
      delay(1000);
      Wire.begin();
      Serial.begin(BAUD_RATE);
      // initialize device
      Serial.println("Initializing I2C devices...");
      accel.initialize();
    }

    if (retries >= RETRIES_MAX)
      exit(EXIT_FAILURE);

    //accel_zero_km_calibration();
    accel.setXAccelOffset(-1877);
    accel.setYAccelOffset(215);
    accel.setZAccelOffset(1654);
    accel.setXGyroOffset(38);
    accel.setYGyroOffset(-44);
    accel.setZGyroOffset(-67);
    Serial.println("MPU6050 connection successful");
    delay(200);

}

void read_acceleration()
{
  int16_t x_raw;
  int16_t y_raw;
  int16_t z_raw;
  float x_ms;
  float y_ms;
  float z_ms;
  char read_out[40];
  accel.getAcceleration(&x_raw, &y_raw, &z_raw);
  x_ms = x_raw/G2MS;
  y_ms = y_raw/G2MS;
  z_ms = z_raw/G2MS;
  sprintf(read_out, "Acceleration:\n\tX: %2.3f\n\tY: %2.3f\n\tZ: %2.3f\n", x_ms, y_ms, z_ms);
  Serial.print(read_out);
}



/*
  integrate and implement filters to get better positional data
  Articles:
  https://www.researchgate.net/post/How_can_I_avoid_data_drifting_when_integrating_acceleration_signal2
  https://blog.prosig.com/2006/12/07/acceleration-velocity-displacement-spectra-%E2%80%93-omega-arithmetic/
  https://dsp.stackexchange.com/questions/320/is-a-kalman-filter-suitable-to-filter-projected-points-positions-given-euler-an/321#321
  https://www.researchgate.net/post/Numerical-integration-of-data-from-acceleration-to-displacement-in-time-domain-or-frequency-domain

*/
void read_acceleration_2()
{
  //accel.
  
}
