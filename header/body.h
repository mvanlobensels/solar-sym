#include <string>
#include <vector>
#include <Eigen/Dense>

#pragma once

class Body 
{
private:
    std::string name_;
    double mass_;

public:
    Eigen::Vector2d position_;
    Eigen::Vector2d velocity_;
    std::vector<Eigen::Vector2d> trajectory_;

    /* Constructor */
    Body(std::string name, double mass, Eigen::Vector2d position, Eigen::Vector2d velocity) 
        : name_(name), mass_(mass), position_(position), velocity_(velocity) {};

    /* Getters */
    const std::string& name() const { return name_; }
    const double mass() const { return mass_; }
};