#pragma once

#include <system.h>
#include <body.h>

namespace solar_system {
constexpr double AU = 1.5e11;  
constexpr double Me = 5.972e24;

/**
 * Creates a System object representing the solar system: the Sun, the eight planets and Pluto,
 * on circular orbits, with the Sun's velocity set so the total momentum is zero.
 */
System make_solar_system();

}; // namespace solar_system