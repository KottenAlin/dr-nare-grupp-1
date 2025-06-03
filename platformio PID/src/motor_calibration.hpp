#ifndef MOTOR_CALIBRATION_HPP
#define MOTOR_CALIBRATION_HPP

#include <Arduino.h>
#include <ESP32Servo.h>
#include <input.hpp>

class MotorCalibration
{
private:
    Servo *esc;
    Input *input;

    // Calibration mode variables
    bool calibrationMode;
    bool testingMode;
    unsigned long lastButtonPress;
    static const unsigned long BUTTON_DEBOUNCE = 500; // 500ms debounce

    // Throttle constants
    static const int BASE_THROTTLE = 1400;
    static const int MIN_THROTTLE = 1100;
    static const int MAX_THROTTLE = 1700;
    static const int MEDIUM_THROTTLE = 1400;

public:
    MotorCalibration(Servo *escArray, Input *inputController);

    void initialize();
    void handleCalibrationInput();
    bool isCalibrationMode() const { return calibrationMode; }

    // Calibration functions
    void performMotorCalibration();
    void testMotorsAtMinSpeed();
    void testMotorsAtMediumSpeed();
    void testIndividualMotor(int motorIndex, int speed);
    void progressiveMotorTest();
    void motorDirectionTest();
    void safetyCheck();
    void displaySystemStatus();
    void printMotorStatus(const char *status);
    void printCalibrationInstructions();
    void emergencyStop();
};

#endif // MOTOR_CALIBRATION_HPP
