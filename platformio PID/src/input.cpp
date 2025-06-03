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
    double raw = PS4.RStickY();
    if (raw < 20) // deadzone
        return 0.0;
    return raw / 4.0; // [-32, 32]
}
double Input::getRoll()
{
    double raw = PS4.RStickX();
    if (raw < 20) // deadzone
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
    double raw = PS4.R1() - PS4.L1();
    return 32.0 * raw; // [-32, 32]
}