#include <input.hpp>

void Input::begin()
{
    if (!PS4.begin("20:43:A8:65:5D:DE"))
    {
        Serial.println("Failed to connect to PS4 controller");
        while (1)
            ;
    }
    Serial.println("Connected to PS4 controller");
}

bool Input::isConnected()
{
    return PS4.isConnected();
}

double Input::getPitch()
{
    double raw = PS4.RStickY(); // Changed from R1/L1 to right stick Y-axis
    if (raw < 20)               // deadzone
        return 0.0;
    return raw / 4.0; // [-32, 32]
}
double Input::getRoll()
{
    double raw = PS4.RStickX(); // Changed from R1/L1 to right stick X-axis
    if (raw < 20)               // deadzone
        return 0.0;
    return raw / 4.0; // [-32, 32]
}
double Input::getThrottle()
{
    double raw = PS4.L2Value() - PS4.R2Value();
    if (raw < 40) // deadzone
        return 0.0;
    return 2.0 * raw; // [-255, 255]
}
double Input::getYaw()
{
    double raw = PS4.LStickX(); // Changed from R1/L1 to left stick X-axis
    if (abs(raw) < 20)          // deadzone
        return 0.0;
    return raw / 4.0; // [-32, 32]
}

// Calibration helper methods
bool Input::getCalibrationButton()
{
    return PS4.Square();
}

bool Input::getEmergencyStop()
{
    return PS4.Circle();
}

bool Input::increaseThrottle()
{
    return PS4.Triangle(); // Triangle button for increasing throttle
}

bool Input::decreaseThrottle()
{
    return PS4.Cross(); // Cross button for decreasing throttle
}

/*bool Input::getMinSpeedTest()
{
    return PS4.Square();
}

bool Input::getMediumSpeedTest()
{
    return PS4.Circle();
}*/

int Input::getIndividualMotorTest()
{
    if (PS4.Up())
        return 0; // Front Vertical
    if (PS4.Right())
        return 1; // Front Horizontal
    if (PS4.Down())
        return 2; // Back Vertical
    if (PS4.Left())
        return 3; // Back Horizontal
    return -1;    // No motor selected
}

bool Input::getProgressiveTest()
{
    return PS4.L1(); // L1 button for progressive test
}

bool Input::getDirectionTest()
{
    return PS4.R1(); // R1 button for direction test
}