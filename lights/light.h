#pragma once
#include <glm/glm.hpp>
#include "../transforms.h"

class Light {
protected:
    Transform transform;
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

public:
    Light(const glm::vec3& pos, const glm::vec3& col, float intens = 1.0f)
        : position(pos)
        , color(col)
        , intensity(intens)
    {
        transform.modelMatrix = glm::translate(glm::mat4(1.0f), position);
    }

    virtual ~Light() = default;

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }

    // Calculate light contribution at a point
    virtual glm::vec3 calculateLightingAt(const glm::vec3& point, const glm::vec3& normal) const {
        // Calculate direction from point to light
        glm::vec3 lightDir = glm::normalize(position - point);
        
        // Calculate distance attenuation (inverse square law)
        float distance = glm::length(position - point);
        float attenuation = 1.0f / (distance * distance);
        
        // Calculate basic diffuse lighting
        float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
        
        // Return final light contribution
        return color * intensity * diff * attenuation;
    }

    // Transform methods
    void setPosition(const glm::vec3& pos) {
        position = pos;
        transform.modelMatrix = glm::translate(glm::mat4(1.0f), position);
    }
}; 