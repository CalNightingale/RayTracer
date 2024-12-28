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
    sphereMaterial.color = glm::vec3(1.0f, 0.0f, 0.0f);  // red
    sphereMaterial.ambient = glm::vec3(0.1f);            // low ambient
    sphereMaterial.diffuse = glm::vec3(0.7f);            // moderate diffuse
    sphereMaterial.specular = glm::vec3(1.0f);           // bright specular highlights
    sphereMaterial.shininess = 32.0f;                    // moderately focused specular
    sphereMaterial.reflectiveness = 0.0f;                // no reflection

    // Add sphere
    auto sphere = std::make_shared<Sphere>(
        glm::vec3(0, 0, 0),    // position
        1.0f,                  // radius
        sphereMaterial         // material
    );
    scene.addShape(sphere);

    // Add floor
    Material floorMaterial;
    floorMaterial.color = glm::vec3(0.5f);               // gray
    floorMaterial.ambient = glm::vec3(0.1f);             // low ambient
    floorMaterial.diffuse = glm::vec3(0.8f);             // high diffuse
    floorMaterial.specular = glm::vec3(0.3f);            // low specular
    floorMaterial.shininess = 16.0f;                     // broad specular highlights
    floorMaterial.reflectiveness = 0.0f;                 // no reflection

    auto floor = std::make_shared<Cuboid>(
        glm::vec3(0, -1.5f, 0),     // position
        glm::vec3(10, 0.1f, 10),    // dimensions
        floorMaterial               // material
    );
    scene.addShape(floor);

    // Create back wall material
    Material wallMaterial;
    wallMaterial.color = glm::vec3(0.9f);               // nearly white
    wallMaterial.ambient = glm::vec3(0.1f);             // low ambient
    wallMaterial.diffuse = glm::vec3(0.3f);             // low diffuse
    wallMaterial.specular = glm::vec3(0.9f);            // high specular
    wallMaterial.shininess = 64.0f;                     // sharp specular highlights
    wallMaterial.reflectiveness = 0.7f;                 // highly reflective

    // Add back wall (using Cuboid)
    auto backWall = std::make_shared<Cuboid>(
        glm::vec3(0, 2.0f, -5.0f),    // position (5 units back from origin)
        glm::vec3(10, 8.0f, 0.1f),    // dimensions (wide, tall, thin)
        wallMaterial                   // material
    );
    scene.addShape(backWall);

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
