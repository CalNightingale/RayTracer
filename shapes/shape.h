#pragma once
#include <glm/glm.hpp>
#include "../ray.h"
#include "../intersection.h"
#include "../transforms.h"
#include "../utils/material.h"

class Shape {
protected:
    Transform transform;
    Material material;

public:
    Shape(const glm::vec3& pos, const Material& mat) 
        : material(mat)
    {
        transform.modelMatrix = glm::translate(glm::mat4(1.0f), pos);
    }
    
    virtual ~Shape() = default;

    // Returns intersection in world space
    virtual Intersection intersect(const Ray& worldRay) const = 0;
    
    // Transform ray from world space to object space
    Ray transformRayToObjectSpace(const Ray& worldRay) const {
        glm::mat4 worldToObject = glm::inverse(transform.modelMatrix);
        glm::vec3 origin = Transform::transformPoint(worldRay.getOrigin(), worldToObject);
        glm::vec3 direction = Transform::transformDirection(worldRay.getDirection(), worldToObject);
        return Ray(origin, direction);
    }

    void setModelMatrix(const glm::mat4& matrix) {
        transform.modelMatrix = matrix;
    }

    glm::mat4 getModelMatrix() const {
        return transform.modelMatrix;
    }
}; 