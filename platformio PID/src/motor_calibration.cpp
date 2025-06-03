#include "motor_calibration.hpp"

MotorCalibration::MotorCalibration(Servo *escArray, Input *inputController)
    : esc(escArray), input(inputController), calibrationMode(false), testingMode(false), lastButtonPress(0)
{
}

void MotorCalibration::initialize()
{
    // Initialize the motor calibration system
    calibrationMode = false;
    testingMode = false;
    lastButtonPress = 0;

    // Initialize all ESCs to minimum throttle for safety
    for (int i = 0; i < 4; i++)
    {
        esc[i].writeMicroseconds(1000);
    }
    delay(2000);

    printCalibrationInstructions();
}

void MotorCalibration::printCalibrationInstructions()
{
    Serial.println("===============================");
    Serial.println("DRONE MOTOR CALIBRATION SYSTEM");
    Serial.println("===============================");
    Serial.println("Controls (PS4 Controller):");
    Serial.println("Triangle: Enter/Exit Calibration Mode");
    Serial.println("Square: Test All Motors at Min Speed");
    Serial.println("Circle: Test All Motors at Medium Speed");
    Serial.println("Cross: Emergency Stop");
    Serial.println("L1: Progressive Speed Test");
    Serial.println("R1: Motor Direction Test");
    Serial.println("D-Pad Up/Down/Left/Right: Test Individual Motors");
    Serial.println("  Up = Front Vertical, Right = Front Horizontal");
    Serial.println("  Down = Back Vertical, Left = Back Horizontal");
    Serial.println("Note: Yaw control moved to Left Stick X-axis");
    Serial.println("===============================");
}

void MotorCalibration::handleCalibrationInput()
{
    unsigned long currentTime = millis();

    // Debounce button presses
    if (currentTime - lastButtonPress < BUTTON_DEBOUNCE)
    {
        return;
    }

    // Check for Triangle button to toggle calibration mode
    if (input->getCalibrationButton())
    {
        calibrationMode = !calibrationMode;
        lastButtonPress = currentTime;

        if (calibrationMode)
        {
            Serial.println("\n=== ENTERING CALIBRATION MODE ===");
            Serial.println("Normal flight disabled for safety");
            performMotorCalibration();
        }
        else
        {
            Serial.println("\n=== EXITING CALIBRATION MODE ===");
            Serial.println("Returning to normal flight mode");
            // Set all motors to minimum throttle when exiting
            emergencyStop();
        }
    }

    // Only process other buttons if in calibration mode
    if (!calibrationMode)
        return;

    // Square button: Test all motors at minimum speed
    if (input->getMinSpeedTest())
    {
        lastButtonPress = currentTime;
        testMotorsAtMinSpeed();
    }

    // Circle button: Test all motors at medium speed
    if (input->getMediumSpeedTest())
    {
        lastButtonPress = currentTime;
        testMotorsAtMediumSpeed();
    }

    // L1 button: Progressive speed test
    if (input->getProgressiveTest())
    {
        lastButtonPress = currentTime;
        progressiveMotorTest();
    }

    // R1 button: Motor direction test
    if (input->getDirectionTest())
    {
        lastButtonPress = currentTime;
        motorDirectionTest();
    }

    // Cross button: Emergency stop
    if (input->getEmergencyStop())
    {
        lastButtonPress = currentTime;
        emergencyStop();
    }

    // D-Pad: Test individual motors
    int motorToTest = input->getIndividualMotorTest();
    if (motorToTest >= 0)
    {
        lastButtonPress = currentTime;
        testIndividualMotor(motorToTest, MEDIUM_THROTTLE);
    }
}

void MotorCalibration::performMotorCalibration()
{
    Serial.println("\n=== STARTING ESC CALIBRATION SEQUENCE ===");

    // Step 1: Set all ESCs to maximum throttle
    Serial.println("Step 1: Setting all ESCs to maximum throttle (1900µs)");
    for (int i = 0; i < 4; i++)
    {
        esc[i].writeMicroseconds(1900);
    }
    printMotorStatus("MAX THROTTLE");
    delay(3000);

    // Step 2: Set all ESCs to minimum throttle
    Serial.println("Step 2: Setting all ESCs to minimum throttle (1000µs)");
    for (int i = 0; i < 4; i++)
    {
        esc[i].writeMicroseconds(1000);
    }
    printMotorStatus("MIN THROTTLE");
    delay(3000);

    Serial.println("=== ESC CALIBRATION COMPLETE ===");
    Serial.println("ESCs should now be calibrated and ready for use");
    Serial.println("\nCalibration Mode Controls:");
    Serial.println("Square: Test all motors at min speed");
    Serial.println("Circle: Test all motors at medium speed");
    Serial.println("L1: Progressive speed test (gradual increase)");
    Serial.println("R1: Motor direction test (individual motor check)");
    Serial.println("Cross: Emergency stop");
    Serial.println("D-Pad: Test individual motors");
    Serial.println("Triangle: Exit calibration mode");
}

void MotorCalibration::testMotorsAtMinSpeed()
{
    Serial.println("\n=== TESTING ALL MOTORS AT MINIMUM SPEED ===");
    for (int i = 0; i < 4; i++)
    {
        esc[i].writeMicroseconds(MIN_THROTTLE);
    }
    printMotorStatus("MIN SPEED TEST");

    delay(3000); // Run for 3 seconds

    // Return to safety position
    emergencyStop();
    Serial.println("Min speed test complete - motors stopped");
}

void MotorCalibration::testMotorsAtMediumSpeed()
{
    Serial.println("\n=== TESTING ALL MOTORS AT MEDIUM SPEED ===");
    Serial.println("WARNING: Motors will spin at medium speed!");

    for (int i = 0; i < 4; i++)
    {
        esc[i].writeMicroseconds(MEDIUM_THROTTLE);
    }
    printMotorStatus("MEDIUM SPEED TEST");

    delay(3000); // Run for 3 seconds

    // Return to safety position
    emergencyStop();
    Serial.println("Medium speed test complete - motors stopped");
}

void MotorCalibration::testIndividualMotor(int motorIndex, int speed)
{
    if (motorIndex < 0 || motorIndex > 3)
        return;

    const char *motorNames[4] = {"Front Vertical", "Front Horizontal", "Back Vertical", "Back Horizontal"};

    Serial.printf("\n=== TESTING MOTOR %d (%s) ===\n", motorIndex, motorNames[motorIndex]);

    // Stop all motors first
    emergencyStop();
    delay(500);

    // Start the selected motor
    esc[motorIndex].writeMicroseconds(speed);
    Serial.printf("Motor %d spinning at %d µs\n", motorIndex, speed);

    delay(2000); // Run for 2 seconds

    // Stop the motor
    esc[motorIndex].writeMicroseconds(1000);
    Serial.printf("Motor %d stopped\n", motorIndex);
}

void MotorCalibration::progressiveMotorTest()
{
    Serial.println("\n=== PROGRESSIVE MOTOR TEST ===");
    Serial.println("Testing all motors with gradual speed increase");

    // Start from minimum throttle and gradually increase
    for (int speed = MIN_THROTTLE; speed <= MEDIUM_THROTTLE; speed += 50)
    {
        Serial.printf("Setting all motors to %d µs\n", speed);
        for (int i = 0; i < 4; i++)
        {
            esc[i].writeMicroseconds(speed);
        }
        delay(1000); // Hold each speed for 1 second
    }

    // Gradually decrease back to stop
    for (int speed = MEDIUM_THROTTLE; speed >= 1000; speed -= 50)
    {
        Serial.printf("Setting all motors to %d µs\n", speed);
        for (int i = 0; i < 4; i++)
        {
            esc[i].writeMicroseconds(speed);
        }
        delay(500);
    }

    Serial.println("Progressive motor test complete");
}

void MotorCalibration::motorDirectionTest()
{
    Serial.println("\n=== MOTOR DIRECTION TEST ===");
    Serial.println("Testing motors one by one to verify direction");

    const char *motorNames[4] = {"Front Vertical (CCW)", "Front Horizontal (CW)",
                                 "Back Vertical (CCW)", "Back Horizontal (CW)"};

    for (int i = 0; i < 4; i++)
    {
        Serial.printf("Testing Motor %d: %s\n", i, motorNames[i]);

        // Start motor gradually
        for (int speed = 1100; speed <= 1300; speed += 50)
        {
            esc[i].writeMicroseconds(speed);
            delay(300);
        }

        delay(1000); // Run at test speed for 1 second

        // Stop motor gradually
        for (int speed = 1300; speed >= 1000; speed -= 50)
        {
            esc[i].writeMicroseconds(speed);
            delay(100);
        }

        Serial.printf("Motor %d test complete\n", i);
        delay(1000); // Pause between motors
    }

    Serial.println("Motor direction test complete");
}

void MotorCalibration::emergencyStop()
{
    Serial.println("EMERGENCY STOP - All motors to minimum");
    for (int i = 0; i < 4; i++)
    {
        esc[i].writeMicroseconds(1000);
    }
    printMotorStatus("STOPPED");
}

void MotorCalibration::safetyCheck()
{
    static unsigned long lastSafetyCheck = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastSafetyCheck > 5000) // Check every 5 seconds
    {
        lastSafetyCheck = currentTime;

        if (!input->isConnected())
        {
            Serial.println("Safety: Controller disconnected - stopping all motors");
            emergencyStop();
        }
    }
}

void MotorCalibration::displaySystemStatus()
{
    static unsigned long lastDisplay = 0;
    unsigned long currentTime = millis();

    // Only display status every 10 seconds to avoid spam
    if (currentTime - lastDisplay > 10000)
    {
        lastDisplay = currentTime;

        Serial.println("\n=== SYSTEM STATUS ===");
        Serial.printf("Calibration Mode: %s\n", calibrationMode ? "ACTIVE" : "INACTIVE");
        Serial.printf("Controller Connected: %s\n", input->isConnected() ? "YES" : "NO");
        Serial.printf("Base Throttle: %d µs\n", BASE_THROTTLE);
        Serial.printf("Min Throttle: %d µs\n", MIN_THROTTLE);
        Serial.printf("Max Throttle: %d µs\n", MAX_THROTTLE);
        Serial.println("====================");
    }
}

void MotorCalibration::printMotorStatus(const char *status)
{
    Serial.println("--------------------------------");
    Serial.printf("MOTOR STATUS: %s\n", status);
    Serial.println("Motor Layout:");
    Serial.println("  [0] FV    [1] FH");
    Serial.println("       \\    /");
    Serial.println("        \\  /");
    Serial.println("         \\/");
    Serial.println("         /\\");
    Serial.println("        /  \\");
    Serial.println("       /    \\");
    Serial.println("  [2] BV    [3] BH");
    Serial.println("--------------------------------");
}
