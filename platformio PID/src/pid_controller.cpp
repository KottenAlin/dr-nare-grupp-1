#include <pid_controller.hpp>
PIDController::PIDController(double *k, double const throttleFactors[4])
    : pid(&input, &output, &setpoint, k[0], k[1], k[2], DIRECT)
{
    setpoint = 0;
    for (int i = 0; i < 4; ++i)
    {
        throttles[i] = throttleFactors[i];
    }
}

void PIDController::initialize()
{
    pid.SetMode(AUTOMATIC);
    pid.SetOutputLimits(-200, 200);
}

void PIDController::setInput(double newInput)
{
    input = newInput;
}

double *PIDController::getOutputs()
{
    pid.Compute();
    for (int i = 0; i < 4; ++i)
    {
        outputs[i] = throttles[i] * output;
    }
    return outputs;
}

void PIDController::setSetpoint(double newSetpoint)
{
    setpoint = newSetpoint;
}