#pragma once
#include <glm/glm.hpp>

struct Intersection {
    bool hit;           // Whether there was an intersection
    float distance;     // Distance to intersection point
    glm::vec3 color;   // Color at intersection point

    Intersection() 
        : hit(false)
        , distance(-1.0f)
        , color(0.0f)
    {}

    Intersection(float dist, const glm::vec3& col)
        : hit(true)
        , distance(dist)
        , color(col)
    {}
}; 