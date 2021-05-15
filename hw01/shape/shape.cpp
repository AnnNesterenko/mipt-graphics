// Include standard headers
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define PI 3.14159265358979323846

#include <common/shader.hpp>

int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int kWinWidth = 1024;
    const int kWinHeight = 768;

    // Open a window and create its OpenGL context
    GLFWwindow *window;
    window = glfwCreateWindow(kWinWidth, kWinHeight, "Triangles", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Background
    glClearColor(1.0, 1.0, 0.9, 0.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create and compile shader program
    GLuint program = LoadShaders("VertexShader.glsl", "FragmentShader.glsl");

    //Triangles vertices
    const float vertex_data[] = {
        -1.0, 0.7, 0.0,
        0.0, 0.7, -1.0,
        1.0, 0.7, 0.0,
        0.0, 0.7, 1.0,

        -1.0, -0.7, 0.0,
        0.0, -0.7, -1.0,
        1.0, -0.7, 0.0,
        0.0, -0.7, 1.0,

        -0.5, 0.0, 0.5,
        -0.5, 0.0, -0.5,
        0.5, 0.0, -0.5,
        0.5, 0.0, 0.5,
        0.0, 0.7, 0.0,
        0.0, -0.7, 0.0,
    };

    const float color_data[] = {
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,

        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,

        0.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0,
    };

    const unsigned int element_data[] = {
        0, 12, 8,
        0, 12, 9,
        0, 8, 9,

        1, 12, 9,
        1, 12, 10,
        1, 9, 10,

        2, 12, 10,
        2, 12, 11,
        2, 10, 11,

        3, 12, 11,
        3, 12, 8,
        3, 11, 8,

        4, 13, 8,
        4, 13, 9,
        4, 8, 9,

        5, 13, 9,
        5, 13, 10,
        5, 9, 10,

        6, 13, 10,
        6, 13, 11,
        6, 10, 11,

        7, 13, 11,
        7, 13, 8,
        7, 11, 8,
    };

    // Camera parameters
    const float kCamRotFreq = 0.1f; //rotations per second
    const float kCamRadius = 5.0f;
    const float kCamDegree = 45.0f;

    // Set projection and model
    glm::mat4 proj = glm::perspective(
        glm::radians(kCamDegree),
        (float) kWinWidth / kWinHeight,
        0.1f,
        100.0f
    );
    glm::mat4 model = glm::mat4(1.0f);

    // Create and bind VAO
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // Create and bind VBO
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(0);

    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(1);

    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_data), element_data, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float cam_x = (float) glm::sin(2 * PI * glfwGetTime() * kCamRotFreq) * kCamRadius;
        float cam_z = (float) glm::cos(2 * PI * glfwGetTime() * kCamRotFreq) * kCamRadius;
        glm::mat4 view = glm::lookAt(
            glm::vec3(cam_x, 0.0, cam_z),
            glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 1.0, 0.0)
        );

        glm::mat4 mvp = proj * view * model;

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    // Cleanup
    glDeleteProgram(program);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteVertexArrays(1, &vertex_array);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}