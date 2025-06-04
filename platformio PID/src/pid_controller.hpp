#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

#include <PID_v1.h>

class PIDController
{
private:
    double input, output, setpoint;
    PID pid;
    int throttles[4];
    double outputs[4];

public:
    PIDController(double *k, double const throttleFactors[4]);
    void initialize();
    void setInput(double newInput);
    void setSetpoint(double newSetpoint);
    double *getOutputs();
    double getOutput() const { return output; }
};

#endif