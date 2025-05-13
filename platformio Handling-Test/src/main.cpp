#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <ESP32Servo.h>
#include <input.hpp>

const int BASE_THROTTLE = 1300;
const int MIN_THROTTLE = 1100;
const int MAX_THROTTLE = 1700;

MPU6050 mpu = MPU6050(Wire);
Servo esc[4]; // FV, FH, BV, BH
Input input;

double pitch[4] = {-1, -1, 1, 1};
double roll[4] = {1, -1, 1, -1};
double throttle[4] = {1, 1, 1, 1};
double yaw[4] = {1, -1, -1, 1};

void setup()
{
  Serial.begin(115200);
  input.begin();
  // Wire.begin(21, 22);
  // mpu.begin();
  // mpu.calcGyroOffsets();

  // const int escPins[4] = {25, 26, 27, 14};
  // for (int i = 0; i < 4; i++)
  // {
  // esc[i].attach(escPins[i]);
  // }

  // for (int i = 0; i < 16; i++)
  // {
  // esc[i % 4].writeMicroseconds(1000);
  // }
  delay(2000);
}

void loop()
{
  // mpu.update();

  delay(500);

  if (!input.isConnected())
  {
    Serial.println("PS4 controller not connected!");
    return;
  }
  double inputXRaw = input.getPitch();
  double inputYRaw = input.getRoll();
  double inputZRaw = input.getThrottle();
  double inputRRaw = input.getYaw();

  Serial.printf("Raw Inputs - Pitch: %.2f, Roll: %.2f, Throttle: %.2f, Yaw: %.2f\n", inputXRaw, inputYRaw, inputZRaw, inputRRaw);

  double inputX[4], inputY[4], inputZ[4], inputR[4];

  for (int i = 0; i < 4; i++)
  {
    inputX[i] = pitch[i] * inputXRaw;
    inputY[i] = roll[i] * inputYRaw;
    inputZ[i] = throttle[i] * inputZRaw;
    inputR[i] = yaw[i] * inputRRaw;
  }

  int outputs[4];
  for (int i = 0; i < 4; i++)
  {
    outputs[i] = constrain(BASE_THROTTLE + inputX[i] + inputY[i] + inputZ[i] + inputR[i], MIN_THROTTLE, MAX_THROTTLE);
    // esc[i].writeMicroseconds(outputs[i]);
  }
  Serial.printf("ESC[0]: %d  ESC[1]: %d  ESC[2]: %d  ESC[3]: %d\n", outputs[0], outputs[1], outputs[2], outputs[3]);
}
