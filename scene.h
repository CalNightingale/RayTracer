#pragma once
#include <vector>
#include <memory>
#include "camera.h"
#include "shapes/shape.h"
#include "utils/utils.h"

class Scene {
private:
    Camera camera;
    std::vector<std::shared_ptr<Shape>> shapes;

public:
    Scene(const Camera& cam = Camera()) : camera(cam) {}

    void addShape(std::shared_ptr<Shape> shape) {
        shapes.push_back(shape);
    }

    void setCamera(const Camera& cam) {
        camera = cam;
    }

    // Finds the closest intersection with any shape for a given ray
    Intersection findClosestIntersection(const Ray& ray) const;

    // Renders the scene and saves it to a PNG file
    void render(const char* filename, int width, int height);
}; 