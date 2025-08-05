#include "system.h"

void System::update_state(double dt)
{
    for (auto &body : bodies_) {
        Eigen::Vector2d force = Eigen::Vector2d::Zero();

        for (auto &other: bodies_)
        {
            if (&body != &other)
            {
                Eigen::Vector2d r = body.position_ - other.position_;
                force += -G * body.mass() * other.mass() * r / std::pow(r.squaredNorm(), 1.5);
            }
        }
        Eigen::Vector2d acceleration = force / body.mass();
        body.velocity_ += acceleration * dt;
        body.position_ += body.velocity_ * dt;
        body.trajectory_.push_back(body.position_);
    }
}
