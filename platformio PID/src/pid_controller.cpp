#include <pid_controller.hpp>
PIDController::PIDController(double kp, double ki, double kd, double const throttleFactors[4])
    : Kp(kp), Ki(ki), Kd(kd), pid(&input, &output, &setpoint, kp, ki, kd, DIRECT)
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