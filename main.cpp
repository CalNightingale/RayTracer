#include "scene.h"
#include "shapes/sphere.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

bool processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
    bool cameraMoved = false;
    const float cameraSpeed = 2.5f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(camera.getDirection() * cameraSpeed);
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(-camera.getDirection() * cameraSpeed);
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(-glm::normalize(glm::cross(camera.getDirection(), glm::vec3(0, 1, 0))) * cameraSpeed);
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(glm::normalize(glm::cross(camera.getDirection(), glm::vec3(0, 1, 0))) * cameraSpeed);
        cameraMoved = true;
    }
    
    return cameraMoved;
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
    bool needsRender = true;  // Initial render is needed
    
    // Add FPS counter variables
    float fpsUpdateInterval = 0.5f;  // Update FPS display every 0.5 seconds
    float lastFpsUpdate = 0.0f;
    int frameCount = 0;
    float currentFps = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Store old camera position and direction
        glm::vec3 oldPos = scene.getCamera().getPosition();
        glm::vec3 oldDir = scene.getCamera().getDirection();

        // Process input
        bool cameraMoved = processInput(window, scene.getCamera(), deltaTime);

        // Check if camera moved
        if (oldPos != scene.getCamera().getPosition() || 
            oldDir != scene.getCamera().getDirection()) {
            needsRender = true;
        }

        // Only render if needed
        if (needsRender) {
            scene.render(width, height);
            needsRender = false;
        }

        // Always update the display
        scene.updateTexture();
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update FPS counter
        frameCount++;
        if (currentFrame - lastFpsUpdate >= fpsUpdateInterval) {
            currentFps = frameCount / (currentFrame - lastFpsUpdate);
            frameCount = 0;
            lastFpsUpdate = currentFrame;
            
            // Update window title
            std::string title = "Ray Tracer (" + std::to_string(static_cast<int>(currentFps)) + " fps)";
            glfwSetWindowTitle(window, title.c_str());
        }
    }

    glfwTerminate();
    return 0;
}
