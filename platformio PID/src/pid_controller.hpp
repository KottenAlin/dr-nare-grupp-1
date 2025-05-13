#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

#include <PID_v1.h>

class PIDController
{
private:
    double input, output, setpoint;
    double Kp, Ki, Kd;
    PID pid;
    int throttles[4] = {0, 0, 0, 0};
    double outputs[4];

public:
    PIDController(double kp, double ki, double kd, double const throttleFactors[4]);
    void initialize();
    void setInput(double newInput);
    void setSetpoint(double newSetpoint);
    double *getOutputs();
};

#endif