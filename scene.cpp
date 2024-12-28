#include "scene.h"
#include <limits>
#include <vector>
#include <stdexcept>
#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif
#include "utils/progress_bar.h"

Scene::Scene(const Camera& cam) : camera(cam) {}


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
    
    // Global ambient light (you might want to make this configurable)
    glm::vec3 globalAmbient(0.1f);
    totalLight += globalAmbient * material.ambient * material.color;

    // Add contribution from each light
    for (const auto& light : lights) {
        // Calculate light direction and distance
        glm::vec3 lightDir = glm::normalize(light->getPosition() - point);
        float lightDistance = glm::length(light->getPosition() - point);
        
        // Cast shadow ray
        Ray shadowRay(point + normal * 0.001f, lightDir);
        Intersection shadowIntersection = findClosestIntersection(shadowRay);
        
        // Check if point is in shadow
        if (!shadowIntersection.hit || shadowIntersection.distance > lightDistance) {
            // Diffuse term (Lambert's law)
            float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
            glm::vec3 diffuse = material.diffuse * diff * material.color * light->getColor();
            
            // Specular term (Phong reflection)
            glm::vec3 viewDir = glm::normalize(camera.getPosition() - point);
            glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
            float spec = pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), material.shininess);
            glm::vec3 specular = material.specular * spec * light->getColor();
            
            // Light attenuation (inverse square law with smoothing)
            float attenuation = 1.0f / (1.0f + 0.09f * lightDistance + 0.032f * lightDistance * lightDistance);
            
            // Add light contribution
            totalLight += (diffuse + specular) * light->getIntensity() * attenuation;
        }
    }

    // Clamp final color to prevent overflow
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
        Ray reflectionRay(hitPoint + intersection.normal * 0.001f, reflectionDir);
        
        glm::vec3 reflectionColor = traceRay(reflectionRay, depth + 1);
        
        // Mix base color with reflection based on reflectiveness
        return glm::mix(baseColor, reflectionColor, intersection.material.reflectiveness);
    }
    
    return baseColor;
}

bool Scene::renderToPNG(const char* filename, int width, int height) {
    std::vector<unsigned char> frameBuffer(width * height * 3);
    ProgressBar progress(height);  // Create progress bar for number of rows

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
        progress.update(y + 1);  // Update progress after each row
    }

    return write_png(filename, width, height, frameBuffer);
}