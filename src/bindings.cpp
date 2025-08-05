#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include "body.h"
#include "system.h"

namespace py = pybind11;

PYBIND11_MODULE(solar_sym, m) {
    py::class_<Body>(m, "Body", py::dynamic_attr())
        .def(py::init([](const std::string& name,
                         double mass,
                         py::object position,
                         py::object velocity) {
            // Convert position and velocity to Eigen::Vector2d
            auto pos_arr = py::array_t<double>(position);
            auto vel_arr = py::array_t<double>(velocity);

            if (pos_arr.size() != 2 || vel_arr.size() != 2)
                throw std::runtime_error("position and velocity must have 2 elements");

            Eigen::Vector2d pos;
            Eigen::Vector2d vel;
            auto pos_ptr = pos_arr.unchecked<1>();
            auto vel_ptr = vel_arr.unchecked<1>();
            for (ssize_t i = 0; i < 2; ++i) {
                pos[i] = pos_ptr[i];
                vel[i] = vel_ptr[i];
            }
            return Body(name, mass, pos, vel);
        }),
        py::arg("name"),
        py::arg("mass"),
        py::arg("position"),
        py::arg("velocity"))
        .def_property_readonly("name", &Body::name)
        .def_property_readonly("mass", &Body::mass)
        .def_readwrite("position", &Body::position_)
        .def_readwrite("velocity", &Body::velocity_)
        .def_readwrite("trajectory", &Body::trajectory_)
        .def("cache_trajectory", [](py::object self) {
            Body& b = self.cast<Body&>();

            size_t N = b.trajectory_.size();
            Eigen::MatrixXd mat(N, 2);

            for (size_t i = 0; i < N; ++i) {
                mat(i, 0) = b.trajectory_[i].x();
                mat(i, 1) = b.trajectory_[i].y();
            }

            self.attr("cached_trajectory") = py::cast(mat);
        });

    py::class_<System>(m, "System")
        .def(py::init<std::vector<Body>>())
        .def("update_state", &System::update_state)
        .def_readwrite("bodies", &System::bodies_)
        .def_property_readonly("G", [](const System& self) { return self.G; })
        .def_property_readonly("AU", [](const System& self) { return self.AU; });
}