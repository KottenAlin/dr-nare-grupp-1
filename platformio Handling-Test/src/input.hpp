#ifndef INPUT_HPP
#define INPUT_HPP

#include <PS4Controller.h>

class Input
{
public:
    void begin();
    bool isConnected();
    double getPitch();
    double getRoll();
    double getThrottle();
    double getYaw();
};

#endif