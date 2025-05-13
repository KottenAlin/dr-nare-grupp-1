#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <Servo.h>
#include <pid_controller.hpp>

const int BASE_THROTTLE = 1300;
const int MIN_THROTTLE = 1100;
const int MAX_THROTTLE = 1700;

MPU6050 mpu;
Servo esc[4]; // FV, FH, BV, BH

double pitch[4] = {-1, -1, 1, 1};
double roll[4] = {-1, 1, -1, 1};
double altitude[4] = {1, 1, 1, 1};
PIDController pidX(1.2, 0.05, 0.25, pitch);
PIDController pidY(1.2, 0.05, 0.25, roll);
PIDController pidZ(1.2, 0.05, 0.25, altitude);

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

  pidX.initialize();
  pidY.initialize();
  pidZ.initialize();
}

void loop()
{
  mpu.update();
  pidX.setInput(mpu.getAngleX());
  pidY.setInput(mpu.getAngleY());
  pidZ.setInput(mpu.getAngleZ());

  double *outputX = pidX.getOutputs();
  double *outputY = pidY.getOutputs();
  double *outputZ = pidZ.getOutputs();

  for (int i = 0; i < 4; i++)
  {
    int output = constrain(BASE_THROTTLE + outputX[i] + outputY[i] + outputZ[i], MIN_THROTTLE, MAX_THROTTLE);
    esc[i].writeMicroseconds(output);
    Serial.printf("ESC[%d]: %d\n", i, output);
  }

  delay(10);
}
