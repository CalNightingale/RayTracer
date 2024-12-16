#pragma once
#include <glm/glm.hpp>

struct Intersection {
    bool hit;           // Whether there was an intersection
    float distance;     // Distance to intersection point
    Material material;    // Base color at intersection point
    glm::vec3 normal;   // Surface normal at intersection point

    Intersection() 
        : hit(false)
        , distance(-1.0f)
        , material(Material())
        , normal(0.0f)
    {}

    Intersection(float dist, const Material& mat, const glm::vec3& norm)
        : hit(true)
        , distance(dist)
        , material(mat)
        , normal(norm)
    {}
}; 