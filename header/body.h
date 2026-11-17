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
    Eigen::Vector3d position_;
    Eigen::Vector3d velocity_;

    Body(std::string name, double mass, Eigen::Vector3d position, Eigen::Vector3d velocity) 
        : name_(name), mass_(mass), position_(position), velocity_(velocity) {};

    const std::string&  name() const { return name_; }
    double              mass() const { return mass_; }
};