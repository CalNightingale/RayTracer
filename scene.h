#pragma once
#include <vector>
#include <memory>
#include "camera.h"
#include "shapes/shape.h"
#include "lights/light.h"
#include "utils/utils.h"

class Scene {
private:
    Camera camera;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<unsigned char> frameBuffer;
    unsigned int textureId;
    int width;
    int height;

public:
    Scene(const Camera& cam = Camera());
    ~Scene();

    void addShape(std::shared_ptr<Shape> shape) {
        shapes.push_back(shape);
    }

    void addLight(std::shared_ptr<Light> light) {
        lights.push_back(light);
    }

    void setCamera(const Camera& cam) {
        camera = cam;
    }

    Camera& getCamera() { return camera; }

    // Finds the closest intersection with any shape for a given ray
    Intersection findClosestIntersection(const Ray& ray) const;

    // Renders the scene to the framebuffer
    void render(int width, int height);
    
    // Updates the OpenGL texture with the current framebuffer
    void updateTexture();

    // Initializes OpenGL resources
    void initializeGL(int width, int height);

    // Calculate total lighting at a point
    glm::vec3 calculateLighting(const glm::vec3& point, const glm::vec3& normal, const Material& material) const;
}; 