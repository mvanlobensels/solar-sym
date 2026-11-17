#include "galaxy.h"

#include <cmath>
#include <random>
#include <utility>
#include <vector>

System galaxy::make_galaxy(std::size_t n, unsigned seed)
{
    const double G = 6.67e-11;
    const double pi = std::acos(-1.0);

    const double stellar_mass  = 6e10 * kSolarMass;
    const double bh_mass       = 4.3e6 * kSolarMass;
    const double scale_length  = 11'400 * kLightYear;   // ~3.5 kpc
    const double scale_height  = 1'000 * kLightYear;    // ~300 pc
    const double max_radius    = 50'000 * kLightYear;
    const double star_mass     = stellar_mass / static_cast<double>(n);

    std::mt19937_64 rng(seed);
    // Surface density ∝ exp(-r/Rd) means p(r) ∝ r·exp(-r/Rd): a Gamma(2, Rd) distribution.
    std::gamma_distribution<double> radius(2.0, scale_length);
    std::exponential_distribution<double> height(1.0 / scale_height);
    std::uniform_real_distribution<double> angle(0.0, 2.0 * pi);
    std::bernoulli_distribution above(0.5);
    std::normal_distribution<double> jitter(0.0, 0.05);

    std::vector<Body> bodies;
    bodies.reserve(n + 1);
    bodies.emplace_back("Sgr A*", bh_mass, Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero());

    for (std::size_t i = 0; i < n; ++i) {
        double r;
        do { r = radius(rng); } while (r > max_radius);

        const double phi = angle(rng);
        const double z = above(rng) ? height(rng) : -height(rng);

        // Mass inside r, treating the disc as if it were spherical (rough, but fine here).
        const double x = r / scale_length;
        const double enclosed = bh_mass + stellar_mass * (1.0 - (1.0 + x) * std::exp(-x));
        const double v = std::sqrt(G * enclosed / r) * (1.0 + jitter(rng));

        bodies.emplace_back("star", star_mass,
                            Eigen::Vector3d(r * std::cos(phi), r * std::sin(phi), z),
                            Eigen::Vector3d(-v * std::sin(phi), v * std::cos(phi), 0.0));
    }

    return System(std::move(bodies));
}
