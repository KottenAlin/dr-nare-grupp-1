/*
 * Motor Calibration Example
 *
 * This example demonstrates how to use the MotorCalibration class
 * to calibrate and test drone motors safely.
 *
 * Hardware Setup:
 * - ESP32 board
 * - 4 ESCs connected to pins 33, 26, 27, 25
 * - PS4 controller paired with ESP32
 * - Motors/Propellers (for testing - be careful!)
 *
 * Safety Notes:
 * - Always remove propellers during initial testing
 * - Ensure motors are securely mounted
 * - Have emergency stop readily available
 * - Test in open area away from people
 */

#include <Arduino.h>
#include <ESP32Servo.h>
#include <input.hpp>
#include <motor_calibration.hpp>

// Create instances
Input input;
Servo esc[4];
MotorCalibration motorCalibration(esc, &input);

void setup()
{
    Serial.begin(115200);
    Serial.println("Motor Calibration System Starting...");

    // Initialize input (PS4 controller)
    input.begin();

    // Attach ESCs to pins
    const int escPins[4] = {33, 26, 27, 25}; // FV, FH, BV, BH
    for (int i = 0; i < 4; i++)
    {
        esc[i].attach(escPins[i]);
    }

    // Initialize motor calibration system
    motorCalibration.initialize();

    Serial.println("System ready! Use PS4 controller for calibration.");
}

void loop()
{
    // Safety check every loop
    motorCalibration.safetyCheck();

    // Check controller connection
    if (!input.isConnected())
    {
        Serial.println("Waiting for PS4 controller connection...");
        delay(1000);
        return;
    }

    // Handle calibration input
    motorCalibration.handleCalibrationInput();

    // Display system status periodically
    motorCalibration.displaySystemStatus();

    delay(50); // Small delay to prevent overwhelming the system
}
