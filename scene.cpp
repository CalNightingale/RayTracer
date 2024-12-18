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
            float u = float(x) / float(width);
            float v = float(y) / float(height);
            
            Ray ray = camera.getRay(u, v);
            glm::vec3 color = traceRay(ray, 0);
            
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

glm::vec3 Scene::calculateLighting(const glm::vec3& point, const glm::vec3& normal, const Material& material) const {
    glm::vec3 totalLight(0.0f);
    
    // Add ambient light
    totalLight += material.ambient * material.color;

    // Add contribution from each light
    for (const auto& light : lights) {
        glm::vec3 lightDir = glm::normalize(light->getPosition() - point);
        
        // Calculate diffuse lighting
        float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
        glm::vec3 diffuse = material.diffuse * diff * material.color * light->getColor();
        
        // Calculate specular lighting
        glm::vec3 viewDir = glm::normalize(camera.getPosition() - point);
        glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
        float spec = pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), material.shininess);
        glm::vec3 specular = material.specular * spec * light->getColor() * material.color;
        
        // Add light contribution with attenuation
        float distance = glm::length(light->getPosition() - point);
        // TODO research attenuation formula
        float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);
        
        totalLight += (diffuse + specular) * light->getIntensity() * attenuation;
    }

    // Clamp values to [0,1]
    return glm::clamp(totalLight, 0.0f, 1.0f);
}

glm::vec3 Scene::traceRay(const Ray& ray, int depth) const {
    if (depth >= MAX_REFLECTION_DEPTH) {
        return glm::vec3(0.0f);
    }

    Intersection intersection = findClosestIntersection(ray);
    
    if (!intersection.hit) {
        return glm::vec3(0.0f);  // Background color (black)
    }

    glm::vec3 hitPoint = ray.at(intersection.distance);
    glm::vec3 baseColor = calculateLighting(hitPoint, intersection.normal, intersection.material);
    
    // Handle reflections
    if (intersection.material.reflectiveness > 0.0f) {
        glm::vec3 reflectionDir = glm::reflect(ray.getDirection(), intersection.normal);
        Ray reflectionRay(hitPoint + intersection.normal * 0.001f, reflectionDir);  // Small offset to prevent self-intersection
        
        glm::vec3 reflectionColor = traceRay(reflectionRay, depth + 1);
        
        // Mix base color with reflection based on reflectiveness
        return glm::mix(baseColor, reflectionColor, intersection.material.reflectiveness);
    }
    
    return baseColor;
}