#include "system.h"
#include "body.h"
#include <Eigen/Dense>
#include <iostream>

int main()
{ 
    const double AU = 1.5e11;                   // Astronomical Unit in meters
    const double Me = 5.972e24;                 // Mass of Earth in kg
    const double sec_per_day = 24 * 60 * 60;    // Seconds in a day 
    const double dt = 1 * sec_per_day;          // Time step in seconds
    const double t_end = 365 * 11 * dt;         // Simulation time in seconds (11 years)
    
    System solar_system(std::vector<Body>{
        Body("Sun",     333000 * Me, Eigen::Vector2d(0, 0),          Eigen::Vector2d(0, 0)),
        Body("Mercury", 0.0553 * Me, Eigen::Vector2d(0.4  * AU, 0),  Eigen::Vector2d(0, 47000)),
        Body("Venus",   0.815  * Me, Eigen::Vector2d(0.72 * AU, 0),  Eigen::Vector2d(0, 35000)),
        Body("Earth",   1.0    * Me, Eigen::Vector2d(1.0  * AU, 0),  Eigen::Vector2d(0, 29290)),
        Body("Mars",    0.107  * Me, Eigen::Vector2d(1.5  * AU, 0),  Eigen::Vector2d(0, 24000)),
        Body("Jupiter", 317.8  * Me, Eigen::Vector2d(5.2  * AU, 0),  Eigen::Vector2d(0, 12440))   
    });

    // Simulation loop
    double t = 0.0;
    while (t < t_end)
    {
        solar_system.update_state(dt);
        t += dt;
    }

    // Output the final positions of the bodies
    for (const auto& body : solar_system.bodies_)
    {        std::cout << body.name() << " final position: (" 
                  << body.position_.x() << ", " 
                  << body.position_.y() << ")\n";
    }

    return 0;
}