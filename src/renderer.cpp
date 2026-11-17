#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace {

const char* kVertexShader = R"(
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;
layout(location = 2) in float a_size;

uniform mat4 u_mvp;
out vec4 v_color;

void main()
{
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    gl_PointSize = a_size;
    v_color = a_color;
}
)";

const char* kFragmentShader = R"(
#version 330 core
in vec4 v_color;
uniform bool u_points;
out vec4 frag_color;

void main()
{
    if (u_points) {
        // Turn the square point sprite into a shaded disc.
        vec2 c = gl_PointCoord * 2.0 - 1.0;
        float d = dot(c, c);
        if (d > 1.0) discard;
        frag_color = vec4(v_color.rgb * (1.0 - 0.4 * d), v_color.a);
    } else {
        frag_color = v_color;
    }
}
)";

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
    float size;
};

const Eigen::Vector3f kPalette[] = {
    {1.00f, 0.85f, 0.30f},  // yellow
    {0.70f, 0.70f, 0.70f},  // grey
    {0.95f, 0.75f, 0.45f},  // tan
    {0.35f, 0.60f, 1.00f},  // blue
    {0.95f, 0.40f, 0.25f},  // red
    {0.85f, 0.65f, 0.50f},  // beige
    {0.55f, 0.85f, 0.95f},  // cyan
    {0.40f, 0.50f, 0.95f},  // indigo
};

GLuint compile_shader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Shader compilation failed: ") + log);
    }
    return shader;
}

GLuint link_program(const char* vertex_source, const char* fragment_source)
{
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        glDeleteProgram(program);
        throw std::runtime_error(std::string("Shader linking failed: ") + log);
    }
    return program;
}

Eigen::Matrix4f perspective(float fovy, float aspect, float near, float far)
{
    const float f = 1.0f / std::tan(fovy / 2.0f);
    Eigen::Matrix4f m = Eigen::Matrix4f::Zero();
    m(0, 0) = f / aspect;
    m(1, 1) = f;
    m(2, 2) = (far + near) / (near - far);
    m(2, 3) = 2.0f * far * near / (near - far);
    m(3, 2) = -1.0f;
    return m;
}

Eigen::Matrix4f look_at(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up)
{
    const Eigen::Vector3f f = (center - eye).normalized();
    const Eigen::Vector3f s = f.cross(up).normalized();
    const Eigen::Vector3f u = s.cross(f);

    Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
    m.block<1, 3>(0, 0) = s.transpose();
    m.block<1, 3>(1, 0) = u.transpose();
    m.block<1, 3>(2, 0) = -f.transpose();
    m(0, 3) = -s.dot(eye);
    m(1, 3) = -u.dot(eye);
    m(2, 3) = f.dot(eye);
    return m;
}

// Point size in pixels, growing with log(mass) so the Sun and Jupiter stand out.
float point_size(double mass)
{
    const double earth_mass = 5.972e24;
    const double size = 5.0 + 2.5 * std::log10(mass / earth_mass);
    return static_cast<float>(std::clamp(size, 4.0, 26.0));
}

}  // namespace

Renderer::Renderer(int width, int height, const char* title, RenderOptions options)
    : options_(options), distance_(options.camera_distance)
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialise GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window (OpenGL 3.3 core required)");
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    const GLenum err = glewInit();
    // A GLX-built GLEW reports NO_GLX_DISPLAY under Wayland (EGL context),
    // but the core GL entry points are still loaded, so that one is harmless.
    if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw std::runtime_error(std::string("Failed to initialise GLEW: ") +
                                 reinterpret_cast<const char*>(glewGetErrorString(err)));
    }

    program_ = link_program(kVertexShader, kFragmentShader);
    u_mvp_ = glGetUniformLocation(program_, "u_mvp");
    u_points_ = glGetUniformLocation(program_, "u_points");

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, r)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, size)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    install_callbacks();
}

Renderer::~Renderer()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteProgram(program_);
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Renderer::install_callbacks()
{
    glfwSetWindowUserPointer(window_, this);

    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int, int action, int) {
        auto* self = static_cast<Renderer*>(glfwGetWindowUserPointer(w));
        if (action != GLFW_PRESS) return;
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w, GLFW_TRUE);
        if (key == GLFW_KEY_SPACE) self->paused_ = !self->paused_;
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* w, int button, int action, int) {
        auto* self = static_cast<Renderer*>(glfwGetWindowUserPointer(w));
        if (button != GLFW_MOUSE_BUTTON_LEFT) return;
        self->dragging_ = (action == GLFW_PRESS);
        glfwGetCursorPos(w, &self->last_x_, &self->last_y_);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* w, double x, double y) {
        auto* self = static_cast<Renderer*>(glfwGetWindowUserPointer(w));
        if (!self->dragging_) return;
        self->yaw_ -= static_cast<float>(x - self->last_x_) * 0.005f;
        self->pitch_ += static_cast<float>(y - self->last_y_) * 0.005f;
        self->pitch_ = std::clamp(self->pitch_, -1.55f, 1.55f);
        self->last_x_ = x;
        self->last_y_ = y;
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* w, double, double dy) {
        auto* self = static_cast<Renderer*>(glfwGetWindowUserPointer(w));
        self->distance_ *= std::pow(0.9f, static_cast<float>(dy));
        const float d0 = self->options_.camera_distance;
        self->distance_ = std::clamp(self->distance_, d0 * 0.005f, d0 * 50.0f);
    });
}

void Renderer::draw(const System& system)
{
    const auto& bodies = system.bodies_;
    const double unit = options_.length_unit;

    // Record trails.
    if (options_.trails && trails_.size() != bodies.size())
        trails_.assign(bodies.size(), {});

    if (options_.trails && !paused_) {
        for (std::size_t i = 0; i < bodies.size(); ++i) {
            trails_[i].push_back((bodies[i].position_ / unit).cast<float>());
            if (trails_[i].size() > max_trail_)
                trails_[i].pop_front();
        }
    }

    // Build vertex data: one line strip per trail, then one point per body.
    std::vector<Vertex> vertices;
    std::vector<std::pair<GLint, GLsizei>> strips;

    for (std::size_t i = 0; i < trails_.size(); ++i) {
        const Eigen::Vector3f& c = kPalette[i % std::size(kPalette)];
        const auto& trail = trails_[i];
        const GLint start = static_cast<GLint>(vertices.size());

        for (std::size_t k = 0; k < trail.size(); ++k) {
            const float alpha = 0.6f * static_cast<float>(k + 1) / static_cast<float>(trail.size());
            const Eigen::Vector3f& p = trail[k];
            vertices.push_back({p.x(), p.y(), p.z(), c.x(), c.y(), c.z(), alpha, 1.0f});
        }
        strips.emplace_back(start, static_cast<GLsizei>(trail.size()));
    }

    const GLint points_start = static_cast<GLint>(vertices.size());
    vertices.reserve(vertices.size() + bodies.size());
    for (std::size_t i = 0; i < bodies.size(); ++i) {
        const Eigen::Vector3f& c = kPalette[i % std::size(kPalette)];
        const Eigen::Vector3f p = (bodies[i].position_ / unit).cast<float>();
        const float size = options_.point_size > 0.0f ? options_.point_size : point_size(bodies[i].mass());
        vertices.push_back({p.x(), p.y(), p.z(), c.x(), c.y(), c.z(), 1.0f, size});
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STREAM_DRAW);

    // Camera.
    int fb_width = 0, fb_height = 0;
    glfwGetFramebufferSize(window_, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);
    const float aspect = fb_height > 0 ? static_cast<float>(fb_width) / fb_height : 1.0f;

    const Eigen::Vector3f eye = distance_ * Eigen::Vector3f(
        std::cos(pitch_) * std::cos(yaw_),
        std::cos(pitch_) * std::sin(yaw_),
        std::sin(pitch_));
    const Eigen::Matrix4f mvp =
        perspective(0.8f, aspect, distance_ * 1e-3f, distance_ * 1e3f) *
        look_at(eye, Eigen::Vector3f::Zero(), Eigen::Vector3f::UnitZ());

    // Draw.
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_);
    glUniformMatrix4fv(u_mvp_, 1, GL_FALSE, mvp.data());  // Eigen is column-major, like OpenGL
    glBindVertexArray(vao_);

    glUniform1i(u_points_, GL_FALSE);
    for (const auto& [start, count] : strips)
        if (count >= 2)
            glDrawArrays(GL_LINE_STRIP, start, count);

    glUniform1i(u_points_, GL_TRUE);
    glDrawArrays(GL_POINTS, points_start, static_cast<GLsizei>(bodies.size()));

    glfwSwapBuffers(window_);
    glfwPollEvents();
}
