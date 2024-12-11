#include "scene.h"
#include <limits>
#include <vector>
#include <stdexcept>
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

Scene::Scene(const Camera& cam) : camera(cam), textureId(0) {}

Scene::~Scene() {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
    }
}

void Scene::initializeGL(int w, int h) {
    // Store dimensions
    width = w;
    height = h;

    // Initialize framebuffer with the correct size
    frameBuffer.resize(width * height * 3);

    // Generate and bind texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Initialize empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Scene::render(int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Convert pixel coordinates to [0,1] range
            float u = float(x) / float(width);
            float v = float(y) / float(height);

            // Get ray for this pixel
            Ray ray = camera.getRay(u, v);

            // Find closest intersection
            Intersection intersection = findClosestIntersection(ray);

            glm::vec3 color(0.0f);
            if (intersection.hit) {
                color = calculateLighting(
                    ray.at(intersection.distance),  // intersection point
                    intersection.normal,            // surface normal
                    intersection.color              // base color
                );
            }

            // Convert color to unsigned char (0-255 range)
            int pixel_index = (y * width + x) * 3;
            frameBuffer[pixel_index] = static_cast<unsigned char>(color.r * 255.0f);
            frameBuffer[pixel_index + 1] = static_cast<unsigned char>(color.g * 255.0f);
            frameBuffer[pixel_index + 2] = static_cast<unsigned char>(color.b * 255.0f);
        }
    }
}

void Scene::updateTexture() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Update texture with new framebuffer data
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                   frameBuffer.size() / 3 / height, height,
                   GL_RGB, GL_UNSIGNED_BYTE, frameBuffer.data());

    // Draw fullscreen quad
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

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

glm::vec3 Scene::calculateLighting(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& baseColor) const {
    glm::vec3 totalLight(0.0f);
    
    // Add ambient light
    float ambientStrength = 0.1f;
    totalLight += ambientStrength * baseColor;

    // Add contribution from each light
    for (const auto& light : lights) {
        totalLight += light->calculateLightingAt(point, normal) * baseColor;
    }

    // Clamp values to [0,1]
    return glm::clamp(totalLight, 0.0f, 1.0f);
}