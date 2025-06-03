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
    // Calibration helper methods
    bool getCalibrationButton();
    bool getEmergencyStop();
    bool getMinSpeedTest();
    bool getMediumSpeedTest();
    int getIndividualMotorTest(); // Returns motor index (0-3) or -1 if none
    bool getProgressiveTest();
    bool getDirectionTest();
};

#endif