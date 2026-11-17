#pragma once

// GLEW must be included before any other OpenGL header (GLFW pulls in GL/gl.h).
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <deque>
#include <vector>

#include "system.h"

struct RenderOptions
{
    double length_unit = 1.5e11;   // Metres per world unit (default: 1 AU)
    float camera_distance = 9.0f;  // Initial camera distance, in world units
    bool trails = true;            // Draw orbit trails (one line strip per body)
    float point_size = 0.0f;       // Fixed point size in pixels; 0 = size by mass
};

class Renderer
{
public:
    Renderer(int width, int height, const char* title, RenderOptions options = {});
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool should_close() const { return glfwWindowShouldClose(window_); }
    bool paused() const { return paused_; }

    /**
     * Records the current body positions in the trails, draws one frame,
     * swaps buffers and processes input events.
     */
    void draw(const System& system);

private:
    RenderOptions options_;
    GLFWwindow* window_ = nullptr;
    GLuint program_ = 0;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLint u_mvp_ = -1;
    GLint u_points_ = -1;

    // Trail per body, in world units (float is precise enough at that scale).
    std::vector<std::deque<Eigen::Vector3f>> trails_;
    std::size_t max_trail_ = 5000;

    // Orbit camera around the origin; z is up (the orbital plane is xy).
    float yaw_ = -1.2f;
    float pitch_ = 0.6f;
    float distance_;   // World units
    bool dragging_ = false;
    double last_x_ = 0.0;
    double last_y_ = 0.0;

    bool paused_ = false;

    void install_callbacks();
};
