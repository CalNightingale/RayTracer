#include "scene.h"
#include "shapes/sphere.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

void processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
    const float cameraSpeed = 2.5f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(camera.getDirection() * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(-camera.getDirection() * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(-glm::normalize(glm::cross(camera.getDirection(), glm::vec3(0, 1, 0))) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(glm::normalize(glm::cross(camera.getDirection(), glm::vec3(0, 1, 0))) * cameraSpeed);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    const int width = 800;
    const int height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, "Ray Tracer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Create camera and scene
    Camera camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1));
    Scene scene(camera);

    // Add a sphere
    auto sphere = std::make_shared<Sphere>(
        glm::vec3(0, 0, 0),    // position
        1.0f,                  // radius
        glm::vec3(1, 0, 0)     // color (red)
    );
    scene.addShape(sphere);

    // Initialize OpenGL resources
    scene.initializeGL(width, height);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, scene.getCamera(), deltaTime);

        // Render scene
        scene.render(width, height);
        scene.updateTexture();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
