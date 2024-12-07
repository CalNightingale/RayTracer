#include "scene.h"
#include <limits>
#include <vector>

Intersection Scene::findClosestIntersection(const Ray& ray) const {
    Intersection closest;
    float minDistance = std::numeric_limits<float>::infinity();

    for (const auto& shape : shapes) {
        Intersection intersection = shape->intersect(ray);
        if (intersection.hit && intersection.distance < minDistance) {
            minDistance = intersection.distance;
            closest = intersection;
        }
    }

    return closest;
}

void Scene::render(const char* filename, int width, int height) {
    const int channels = 3; // RGB
    std::vector<unsigned char> image(width * height * channels);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Convert pixel coordinates to [0,1] range
            float u = float(x) / float(width);
            float v = float(y) / float(height);

            // Get ray for this pixel
            Ray ray = camera.getRay(u, v);

            // Find closest intersection
            Intersection intersection = findClosestIntersection(ray);

            // Calculate pixel color
            glm::vec3 color = intersection.hit ? intersection.color : glm::vec3(0.0f);

            // Convert color to unsigned char (0-255 range)
            int pixel_index = (y * width + x) * channels;
            image[pixel_index] = static_cast<unsigned char>(color.r * 255.0f);
            image[pixel_index + 1] = static_cast<unsigned char>(color.g * 255.0f);
            image[pixel_index + 2] = static_cast<unsigned char>(color.b * 255.0f);
        }
    }

    if (!write_png(filename, width, height, image)) {
        throw std::runtime_error("Failed to write PNG file");
    }
} 