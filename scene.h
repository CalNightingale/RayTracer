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
}; 