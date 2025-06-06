#include <pid_manager.hpp>

PIDManager::PIDManager(double **k, double **throttles)
    : pidControllers{
          PIDController(k[0], throttles[0]),
          PIDController(k[1], throttles[1]),
          PIDController(k[2], throttles[2])}
{
}

void PIDManager::initialize()
{
    for (int i = 0; i < 3; ++i)
    {
        pidControllers[i].initialize();
    }
}

void PIDManager::setInputs(double *inputs)
{
    for (int i = 0; i < 3; ++i)
    {
        pidControllers[i].setInput(inputs[i]);
    }
}

void PIDManager::setSetpoints(double *setpoints)
{
    for (int i = 0; i < 3; ++i)
    {
        pidControllers[i].setSetpoint(setpoints[i]);
    }
}

double *PIDManager::getOutputs()
{
    static double outputs[3];
    for (int i = 0; i < 3; ++i)
    {
        double *pidOutput = pidControllers[i].getOutputs();
        outputs[i] = 0;
        for (int j = 0; j < 3; ++j)
        {
            outputs[i] += pidOutput[j];
        }
    }
    return outputs;
}
