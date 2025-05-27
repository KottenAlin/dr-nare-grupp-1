#ifndef PID_MANAGER_HPP
#define PID_MANAGER_HPP

#include <pid_controller.hpp>

class PIDManager
{
public:
    PIDManager(double **k, double **throttles);
    void initialize();
    void setInputs(double *inputs);
    void setSetpoints(double *setpoints);
    double *getOutputs();

private:
    PIDController pidControllers[4];
};

#endif