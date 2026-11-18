#include "solar_system.h"

System solar_system::make_solar_system()
{
    std::vector<Body> bodies{
        Body("Sun",     333000 * Me, Eigen::Vector3d(0, 0, 0),          Eigen::Vector3d(0, 0, 0)),
        Body("Mercury", 0.0553 * Me, Eigen::Vector3d(0.4  * AU, 0, 0),  Eigen::Vector3d(0, 47000, 0)),
        Body("Venus",   0.815  * Me, Eigen::Vector3d(0.72 * AU, 0, 0),  Eigen::Vector3d(0, 35000, 0)),
        Body("Earth",   1.0    * Me, Eigen::Vector3d(1.0  * AU, 0, 0),  Eigen::Vector3d(0, 29750, 0)),
        Body("Mars",    0.107  * Me, Eigen::Vector3d(1.5  * AU, 0, 0),  Eigen::Vector3d(0, 24000, 0)),
        Body("Jupiter", 317.8  * Me, Eigen::Vector3d(5.2  * AU, 0, 0),  Eigen::Vector3d(0, 13050, 0)),
        Body("Saturn",  95.2   * Me, Eigen::Vector3d(9.5  * AU, 0, 0),  Eigen::Vector3d(0, 9690, 0)),
        Body("Uranus",  14.5   * Me, Eigen::Vector3d(19.2 * AU, 0, 0),  Eigen::Vector3d(0, 6810, 0)),
        Body("Neptune", 17.1   * Me, Eigen::Vector3d(30.1 * AU, 0, 0),  Eigen::Vector3d(0, 5430, 0)),
        Body("Pluto",   0.0022 * Me, Eigen::Vector3d(39.5 * AU, 0, 0),  Eigen::Vector3d(0, 4740, 0))
    };

    // Give the Sun the opposite of the planets' total momentum so the
    // system's centre of mass stays at rest instead of drifting.
    Eigen::Vector3d momentum = Eigen::Vector3d::Zero();
    for (size_t i = 1; i < bodies.size(); ++i)
        momentum += bodies[i].mass() * bodies[i].velocity_;
    bodies[0].velocity_ = -momentum / bodies[0].mass();

    return System(bodies);
}
