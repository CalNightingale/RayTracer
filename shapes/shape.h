#pragma once
#include <glm/glm.hpp>
#include "../ray.h"
#include "../intersection.h"

class Shape {
protected:
    glm::vec3 position;
    glm::vec3 color;

public:
    Shape(const glm::vec3& pos, const glm::vec3& col) 
        : position(pos)
        , color(col)
    {}
    virtual ~Shape() = default;

    // Returns distance to intersection point, or -1 if no intersection
    virtual Intersection intersect(const Ray& ray) const = 0;
    
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getColor() const { return color; }
}; 