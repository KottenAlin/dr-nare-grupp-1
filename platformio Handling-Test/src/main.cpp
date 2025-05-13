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

  double inputXRaw = 0;
  double inputYRaw = 0;
  double inputZRaw = 0;
  double inputRRaw = 0;

  double inputX[4], inputY[4], inputZ[4], inputR[4];

  for (int i = 0; i < 4; i++)
  {
    inputX[i] = pitch[i] * inputXRaw;
    inputY[i] = roll[i] * inputYRaw;
    inputZ[i] = throttle[i] * inputZRaw;
    inputR[i] = yaw[i] * inputRRaw;
  }

  for (int i = 0; i < 4; i++)
  {
    int output = constrain(BASE_THROTTLE + inputX[i] + inputY[i] + inputZ[i] + inputR[i], MIN_THROTTLE, MAX_THROTTLE);
    esc[i].writeMicroseconds(output);
    Serial.printf("ESC[%d]: %d\n", i, output);
  }

  delay(10);
}
