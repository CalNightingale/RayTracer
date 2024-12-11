#pragma once
#include <glm/glm.hpp>

struct Intersection {
    bool hit;           // Whether there was an intersection
    float distance;     // Distance to intersection point
    glm::vec3 color;    // Base color at intersection point
    glm::vec3 normal;   // Surface normal at intersection point

    Intersection() 
        : hit(false)
        , distance(-1.0f)
        , color(0.0f)
        , normal(0.0f)
    {}

    Intersection(float dist, const glm::vec3& col, const glm::vec3& norm)
        : hit(true)
        , distance(dist)
        , color(col)
        , normal(norm)
    {}
}; 