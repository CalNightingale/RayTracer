#include "scene.h"
#include "shapes/sphere.h"
#include <memory>
#include <iostream>

int main() {
    const int width = 800;
    const int height = 600;

    // Create camera
    Camera camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1));

    // Create scene
    Scene scene(camera);

    // Add a sphere
    auto sphere = std::make_shared<Sphere>(
        glm::vec3(0, 0, 0),    // position
        1.0f,                  // radius
        glm::vec3(1, 0, 0)     // color (red)
    );
    scene.addShape(sphere);

    // Render scene
    scene.render("output.png", width, height);

    std::cout << "Image saved successfully as 'output.png'." << std::endl;
    return 0;
}
