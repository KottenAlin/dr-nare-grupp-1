#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <Servo.h>

const int BASE_THROTTLE = 1300;
const int MIN_THROTTLE = 1100;
const int MAX_THROTTLE = 1700;

MPU6050 mpu;
Servo esc[4]; // FV, FH, BV, BH

double pitch[4] = {-1, -1, 1, 1};
double roll[4] = {-1, 1, -1, 1};
double throttle[4] = {1, 1, 1, 1};
double yaw[4] = {1, -1, -1, 1};

void setup()
{
  Serial.begin(115200);
  Wire.begin(21, 22);
  mpu.begin();
  mpu.calcGyroOffsets();

  const int escPins[4] = {25, 26, 27, 14};
  for (int i = 0; i < 4; i++)
  {
    esc[i].attach(escPins[i]);
  }

  for (int i = 0; i < 16; i++)
  {
    esc[i % 4].writeMicroseconds(1000);
  }
  delay(2000);
}

void loop()
{
  mpu.update();

  // hantera input

  delay(10);
}
