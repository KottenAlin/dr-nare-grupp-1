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
    return PS4.RStickY();
}
double Input::getRoll()
{
    return PS4.RStickX();
}
double Input::getThrottle()
{
    return PS4.R2Value() - PS4.L2Value();
}
double Input::getYaw()
{
    return PS4.R1() * 100 - PS4.L1() * 100;
}