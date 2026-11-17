#include "system.h"
#include "body.h"
#include "renderer.h"
#include "galaxy.h"
#include <Eigen/Dense>
#include <chrono>
#include <exception>
#include <iostream>

int main()
{
    const double AU = 1.5e11;                   // Astronomical Unit in meters
    const double Me = 5.972e24;                 // Mass of Earth in kg
    const double dt = 60 * 60;                  // Time step in seconds (1 hour)
    const int steps_per_frame = 24;             // Simulated time per frame: 1 day

    // System solar_system(std::vector<Body>{
    //     Body("Sun",     333000 * Me, Eigen::Vector3d(0, 0, 0),          Eigen::Vector3d(0, 0, 0)),
    //     Body("Mercury", 0.0553 * Me, Eigen::Vector3d(0.4  * AU, 0, 0),  Eigen::Vector3d(0, 47000, 0)),
    //     Body("Venus",   0.815  * Me, Eigen::Vector3d(0.72 * AU, 0, 0),  Eigen::Vector3d(0, 35000, 0)),
    //     Body("Earth",   1.0    * Me, Eigen::Vector3d(1.0  * AU, 0, 0),  Eigen::Vector3d(0, 29290, 0)),
    //     Body("Mars",    0.107  * Me, Eigen::Vector3d(1.5  * AU, 0, 0),  Eigen::Vector3d(0, 24000, 0)),
    //     Body("Jupiter", 317.8  * Me, Eigen::Vector3d(5.2  * AU, 0, 0),  Eigen::Vector3d(0, 12440, 0))
    // });

    // Milky Way: one million stars. step() is O(N²), so expect this to crawl.
    const std::size_t n_stars = 10000;
    const double star_dt = 1e5 * 365.25 * 86400;   // Time step: 100,000 years
    const int star_steps_per_frame = 1;

    std::cout << "Generating " << n_stars << " stars..." << std::endl;
    System milky_way = galaxy::make_galaxy(n_stars);

    RenderOptions options;
    options.length_unit = galaxy::light_year;   // World units are light years
    options.camera_distance = 120'000.0f;
    options.trails = false;                     // 1M trails = 1M draw calls per frame
    options.point_size = 1.5f;

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
                for (int i = 0; i < star_steps_per_frame; ++i)
                    milky_way.step(star_dt);
                const std::chrono::duration<double> took = std::chrono::steady_clock::now() - start;
                std::cout << "frame " << ++frame << ": step took " << took.count() << " s" << std::endl;
            }

            renderer.draw(milky_way);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}