#pragma once

#include <cstddef>

#include "system.h"

namespace galaxy {

constexpr double light_year = 9.4607e15;   // metres
constexpr double solar_mass = 1.989e30;    // kg

/**
 * Builds a Milky-Way-like disc: a central black hole (Sgr A*) plus n
 * equal-mass "stars" sharing ~6e10 solar masses. Radii follow an exponential
 * disc (scale length ~11,400 ly, cut off at 50,000 ly), heights an
 * exponential profile (~1,000 ly). Every star starts on a circular orbit
 * around the mass enclosed by its radius, with 5% random velocity jitter.
 *
 * There is no dark matter halo, so the rotation curve falls off with radius
 * instead of staying flat like the real Milky Way's.
 *
 * @param n    Number of stars (excluding the black hole).
 * @param seed Random seed, so runs are reproducible.
 */
System make_galaxy(std::size_t n, unsigned seed = 42);

}  // namespace galaxy
