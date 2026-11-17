#include <string>
#include <vector>
#include <Eigen/Dense>
#include "body.h"

#pragma once

class System
{
public:
    const double G = 6.67e-11;
    const double AU = 1.5e11;

    std::vector<Body> bodies_;

    System(std::vector<Body> bodies) : bodies_(bodies) {};

    /**
     * Updates the state of the system by calculating the forces between bodies
     * and updating their positions and velocities.
     * 
     * @param dt Time step in seconds.
     */
    void step(double dt);
};