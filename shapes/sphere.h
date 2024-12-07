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

    Intersection intersect(const Ray& ray) const override {
        glm::vec3 oc = ray.getOrigin() - position;
        float a = glm::dot(ray.getDirection(), ray.getDirection());
        float b = 2.0f * glm::dot(oc, ray.getDirection());
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b*b - 4*a*c;

        if (discriminant < 0) {
            return Intersection(); // No hit
        }

        float dist = (-b - sqrt(discriminant)) / (2.0f*a);
        if (dist < 0) {
            dist = (-b + sqrt(discriminant)) / (2.0f*a);
            if (dist < 0) {
                return Intersection(); // No hit
            }
        }

        return Intersection(dist, color);
    }
};