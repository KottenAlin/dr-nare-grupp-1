#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <ESP32Servo.h>
#include <pid_manager.hpp>
#include <input.hpp>
#include <motor_calibration.hpp>

const int BASE_THROTTLE = 1000;
const int MIN_THROTTLE = 1000;
const int MAX_THROTTLE = 1700;

Input input;
MPU6050 mpu = MPU6050(Wire);
Servo esc[4]; // FV, FH, BV, BH
MotorCalibration motorCalibration(esc, &input);

double kPitch[3] = {1.2, 0.00, 0.0};
double kRoll[3] = {1.2, 0.00, 0.0};
double kYaw[3] = {0.8, 0.000, 0.00};

double pitch[4] = {-1, -1, 1, 1};
double roll[4] = {-1, 1, -1, 1};
double yaw[4] = {1, -1, -1, 1};

double *kParams[3] = {kPitch, kRoll, kYaw};
double *axes[3] = {pitch, roll, yaw};
PIDManager pid(kParams, axes);

void setup()
{
  Serial.begin(115200);
  input.begin();
  Wire.begin(21, 22);
  mpu.begin();
  mpu.calcGyroOffsets();

  const int escPins[4] = {33, 26, 27, 25}; // FV, FH, BV, BH
  for (int i = 0; i < 4; i++)
  {
    esc[i].attach(escPins[i]);
  }
  for (int i = 0; i < 4; i++)
  {
    esc[i].writeMicroseconds(1000);
  }
  delay(2000);

  pid.initialize();
  motorCalibration.initialize(); // Initialize motor calibration system, // which sets up ESCs and prints instructions
  Serial.println("System ready! Use PS4 controller for calibration.");
}

void loop()
{
  // Continuous safety monitoring
  motorCalibration.safetyCheck(); // Check if the controller is connected and stop motors if not

  if (!input.isConnected())
  {
    Serial.println("PS4 controller not connected - waiting for connection...");
    for (int i = 0; i < 4; i++)
    {
      esc[i].writeMicroseconds(1000); // Safety: Set to minimum throttle
    }
    delay(1000);
    return;
  }

  // Handle calibration mode input
  motorCalibration.handleCalibrationInput();

  // If in calibration mode, skip normal flight operations
  if (motorCalibration.isCalibrationMode())
  {
    delay(50);
    return;
  }

  // Normal flight operation
  mpu.update();
  double inputs[3] = {mpu.getAngleX(), mpu.getAngleY(), mpu.getGyroZ()};
  pid.setInputs(inputs);

  double setpoints[3] = {input.getPitch(), input.getRoll(), input.getYaw()};
  pid.setSetpoints(setpoints);

  double *outputs = pid.getOutputs(); // Get PID outputs for pitch, roll, and yaw
  int throttle = constrain(BASE_THROTTLE + input.getThrottle(), MIN_THROTTLE, MAX_THROTTLE);

  // Serial.printf("outputs: P=%.2f, R=%.2f, Y=%.2f, Throttle=%d\n", outputs[0], outputs[1], outputs[2], throttle);

  for (int i = 0; i < 4; i++)
  {
    double motorCorrection = pitch[i] * outputs[0] +
                             roll[i] * outputs[1] +
                             yaw[i] * outputs[2]; // Calculate the correction for each motor based on PID outputs
    int output = constrain(throttle + motorCorrection, MIN_THROTTLE, MAX_THROTTLE);

    esc[i].writeMicroseconds(output);                                                     // Write the calculated output to each ESC
    Serial.printf("Motor %d output: %d, correction: %.2f\n", i, output, motorCorrection); // Print motor outputs and correction for debugging

    delay(10);
  }
  static int printCounter = 0;
  if (printCounter++ % 10 == 0) // Print every 10 iterations
  {
    Serial.printf("Pitch: %.2f, Roll: %.2f, Yaw: %.2f, Throttle: %d\n",
                  mpu.getAngleX(), mpu.getAngleY(), mpu.getGyroZ(), throttle);
    Serial.printf("Outputs: P=%.2f, R=%.2f, Y=%.2f\n", outputs[0], outputs[1], outputs[2]);
  }
}
