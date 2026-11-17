#include "system.h"

void System::step(double dt)
{
    for (auto &body : bodies_) {
        Eigen::Vector3d force = Eigen::Vector3d::Zero();

        for (auto &other: bodies_)
        {
            if (&body != &other)
            {
                Eigen::Vector3d r = body.position_ - other.position_;
                force += -G * body.mass() * other.mass() * r / std::pow(r.squaredNorm(), 1.5);
            }
        }
        Eigen::Vector3d acceleration = force / body.mass();
        body.velocity_ += acceleration * dt;
        body.position_ += body.velocity_ * dt;
        }
}
