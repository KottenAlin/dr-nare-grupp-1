#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <ESP32Servo.h>
#include <pid_manager.hpp>
#include <input.hpp>

const int BASE_THROTTLE = 1300;
const int MIN_THROTTLE = 1100;
const int MAX_THROTTLE = 1700;

Input input;
MPU6050 mpu = MPU6050(Wire);
Servo esc[4]; // FV, FH, BV, BH

double kPitch[3] = {1.2, 0.05, 0.25};
double kRoll[3] = {1.2, 0.05, 0.25};
double kThrottle[3] = {1.2, 0.05, 0.25};
double kYaw[3] = {1.2, 0.05, 0.25};

double pitch[4] = {-1, -1, 1, 1};
double roll[4] = {-1, 1, -1, 1};
double throttle[4] = {1, 1, 1, 1};
double yaw[4] = {1, -1, -1, 1};

double *kParams[4] = {kPitch, kRoll, kThrottle, kYaw};
double *axes[4] = {pitch, roll, throttle, yaw};
PIDManager pid(kParams, axes);

void setup()
{
  Serial.begin(115200);
  input.begin();
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

  pid.initialize();
}

void loop()
{
  mpu.update();
  double inputs[4] = {mpu.getAngleX(), mpu.getAngleY(), mpu.getAngleZ(), mpu.getGyroZ()};
  pid.setInputs(inputs);

  double setpoints[4] = {input.getPitch(), input.getRoll(), input.getThrottle(), input.getYaw()};
  pid.setSetpoints(setpoints);

  double *outputs = pid.getOutputs();

  for (int i = 0; i < 4; i++)
  {
    int output = constrain(BASE_THROTTLE + outputs[i], MIN_THROTTLE, MAX_THROTTLE);
    esc[i].writeMicroseconds(output);
    Serial.printf("ESC[%d]: %d\n", i, output);
  }

  delay(10);
}
