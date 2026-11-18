#include "system.h"
#include "body.h"
#include "renderer.h"
#include "galaxy.h"
#include "solar_system.h"

#include <Eigen/Dense>
#include <chrono>
#include <exception>
#include <iostream>

int main()
{
    const double dt = 60 * 60;                  // Time step in seconds (1 hour)
    const int steps_per_frame = 24;             // Simulated time per frame: 1 day
    System solar_system = solar_system::make_solar_system();

    // Milky Way
    // const std::size_t n_stars = 10000;
    // const double dt = 1e5 * 365.25 * 86400;   // Time step: 100,000 years
    // const int steps_per_frame = 1;

    // std::cout << "Generating " << n_stars << " stars..." << std::endl;
    // System milky_way = galaxy::make_galaxy(n_stars);

    // RenderOptions options;
    // options.length_unit = galaxy::light_year;   // World units are light years
    // options.camera_distance = 120'000.0f;
    // options.trails = false;                     // 1M trails = 1M draw calls per frame
    // options.point_size = 1.5f;

    try
    {
        Renderer renderer(1920, 1080, "solar-sym", options);

        // Simulation + render loop (drag: rotate, scroll: zoom, space: pause, esc: quit)
        int frame = 0;
        while (!renderer.should_close())
        {
            if (!renderer.paused())
            {
                const auto start = std::chrono::steady_clock::now();
                for (int i = 0; i < steps_per_frame; ++i)
                    solar_system.step(dt);
                const std::chrono::duration<double> took = std::chrono::steady_clock::now() - start;
                std::cout << "frame " << ++frame << ": step took " << took.count() << " s" << std::endl;
            }

            renderer.draw(solar_system);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}