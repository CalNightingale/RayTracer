#pragma once
#include <glm/glm.hpp>

class Ray {
private:
    glm::vec3 origin;
    glm::vec3 direction;

public:
    Ray(const glm::vec3& orig, const glm::vec3& dir)
        : origin(orig)
        , direction(glm::normalize(dir))  // Ensure direction is normalized
    {}

    glm::vec3 getOrigin() const { return origin; }
    glm::vec3 getDirection() const { return direction; }

    // Get point along ray at distance t
    glm::vec3 at(float t) const {
        return origin + direction * t;
    }
}; 