#pragma once
#include "shape.h"

class Sphere : public Shape {
private:
    float radius;

public:
    Sphere(const glm::vec3& pos, float r, const glm::vec3& col)
        : Shape(pos, col)
        , radius(r)
    {}

    Intersection intersect(const Ray& worldRay) const override {
        // Transform ray to object space
        Ray objectRay = transformRayToObjectSpace(worldRay);
        
        // Perform intersection in object space
        glm::vec3 oc = objectRay.getOrigin();  // In object space, center is at origin
        float a = glm::dot(objectRay.getDirection(), objectRay.getDirection());
        float b = 2.0f * glm::dot(oc, objectRay.getDirection());
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b*b - 4*a*c;

        if (discriminant < 0) {
            return Intersection();
        }

        float dist = (-b - sqrt(discriminant)) / (2.0f*a);
        if (dist < 0) {
            dist = (-b + sqrt(discriminant)) / (2.0f*a);
            if (dist < 0) {
                return Intersection();
            }
        }

        // Transform intersection point back to world space
        glm::vec3 objectHitPoint = objectRay.at(dist);
        glm::vec3 objectNormal = glm::normalize(objectHitPoint);  // For sphere, normal is just normalized position
        
        // Transform normal to world space
        glm::vec3 worldHitPoint = Transform::transformPoint(objectHitPoint, transform.modelMatrix);
        glm::vec3 worldNormal = glm::normalize(Transform::transformDirection(objectNormal, transform.modelMatrix));
        
        float worldDist = glm::length(worldHitPoint - worldRay.getOrigin());
        return Intersection(worldDist, color, worldNormal);
    }
};