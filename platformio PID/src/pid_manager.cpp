#include <pid_manager.hpp>

PIDManager::PIDManager(double **k, double **throttles)
{
    for (int i = 0; i < 4; ++i)
    {
        pidControllers[i] = PIDController(k[i], throttles[i]);
    }
}

void PIDManager::initialize()
{
    for (int i = 0; i < 4; ++i)
    {
        pidControllers[i].initialize();
    }
}

void PIDManager::setInputs(double *inputs)
{
    for (int i = 0; i < 4; ++i)
    {
        pidControllers[i].setInput(inputs[i]);
    }
}

void PIDManager::setSetpoints(double *setpoints)
{
    for (int i = 0; i < 4; ++i)
    {
        pidControllers[i].setSetpoint(setpoints[i]);
    }
}

double *PIDManager::getOutputs()
{
    static double outputs[4];
    for (int i = 0; i < 4; ++i)
    {
        double *pidOutput = pidControllers[i].getOutputs();
        outputs[i] = 0;
        for (int j = 0; j < 4; ++j)
        {
            outputs[i] += pidOutput[j];
        }
    }
    return outputs;
}