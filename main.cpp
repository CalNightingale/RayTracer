#include "scene.h"
#include "shapes/sphere.h"
#include "shapes/cuboid.h"
#include <memory>
#include <iostream>

int main() {
    const int width = 800;
    const int height = 600;

    // Create camera and scene
    Camera camera(
        glm::vec3(0, 4, 10),     // Position: higher up and further back
        glm::vec3(0, -0.3f, -1)  // Direction: looking slightly downward
    );
    Scene scene(camera);

    // Create a material for the sphere
    Material sphereMaterial;
    sphereMaterial.color = glm::vec3(1, 0, 0);  // red
    sphereMaterial.ambient = 0.1f;
    sphereMaterial.diffuse = 0.7f;
    sphereMaterial.specular = 0.3f;
    sphereMaterial.shininess = 32.0f;
    sphereMaterial.reflectiveness = 0.0f;

    // Add sphere
    auto sphere = std::make_shared<Sphere>(
        glm::vec3(0, 0, 0),    // position
        1.0f,                  // radius
        sphereMaterial         // material
    );
    scene.addShape(sphere);

    // Add floor
    Material floorMaterial;
    floorMaterial.color = glm::vec3(0.5f);  // gray
    floorMaterial.ambient = 0.1f;
    floorMaterial.diffuse = 0.7f;
    floorMaterial.specular = 0.2f;
    floorMaterial.shininess = 16.0f;
    floorMaterial.reflectiveness = 0.0f;

    auto floor = std::make_shared<Cuboid>(
        glm::vec3(0, -1.5f, 0),     // position
        glm::vec3(10, 0.1f, 10),    // dimensions
        floorMaterial               // material
    );
    scene.addShape(floor);

    // Add light
    auto light = std::make_shared<Light>(
        glm::vec3(2, 4, 3),     // position
        glm::vec3(1, 1, 1),     // color (white)
        1.0f                     // intensity
    );
    scene.addLight(light);

    // Render to PNG
    if (!scene.renderToPNG("output.png", width, height)) {
        std::cerr << "Failed to write PNG file" << std::endl;
        return -1;
    }

    std::cout << "Rendered image saved to output.png" << std::endl;
    return 0;
}
