#pragma once
#include "shape.h"
#include <algorithm>

class Cuboid : public Shape {
private:
    glm::vec3 dimensions;  // width, height, depth

public:
    Cuboid(const glm::vec3& pos, const glm::vec3& dims, const Material& mat)
        : Shape(pos, mat)
        , dimensions(dims)
    {
        // Scale the model matrix by the dimensions
        transform.modelMatrix = glm::scale(transform.modelMatrix, dimensions);
    }

    Intersection intersect(const Ray& worldRay) const override {
        // Transform ray to object space
        Ray objectRay = transformRayToObjectSpace(worldRay);
        
        // Calculate intersection with axis-aligned box in object space
        glm::vec3 invDir = 1.0f / objectRay.getDirection();
        
        // Calculate intersections with all slabs
        float t1 = (-0.5f - objectRay.getOrigin().x) * invDir.x;
        float t2 = (0.5f - objectRay.getOrigin().x) * invDir.x;
        float t3 = (-0.5f - objectRay.getOrigin().y) * invDir.y;
        float t4 = (0.5f - objectRay.getOrigin().y) * invDir.y;
        float t5 = (-0.5f - objectRay.getOrigin().z) * invDir.z;
        float t6 = (0.5f - objectRay.getOrigin().z) * invDir.z;

        float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        // If tmax < 0, ray is intersecting box, but box is behind ray's origin
        if (tmax < 0) {
            return Intersection();
        }

        // If tmin > tmax, ray doesn't intersect box
        if (tmin > tmax) {
            return Intersection();
        }

        // Calculate hit point in object space
        glm::vec3 objectHitPoint = objectRay.at(tmin);
        
        // Calculate normal based on which face was hit
        glm::vec3 objectNormal(0.0f);
        float epsilon = 0.0001f; // To handle floating point precision issues
        
        // Check which face was hit by comparing hit point coordinates with box bounds
        if (std::abs(objectHitPoint.x - 0.5f) < epsilon) objectNormal = glm::vec3(1, 0, 0);
        else if (std::abs(objectHitPoint.x + 0.5f) < epsilon) objectNormal = glm::vec3(-1, 0, 0);
        else if (std::abs(objectHitPoint.y - 0.5f) < epsilon) objectNormal = glm::vec3(0, 1, 0);
        else if (std::abs(objectHitPoint.y + 0.5f) < epsilon) objectNormal = glm::vec3(0, -1, 0);
        else if (std::abs(objectHitPoint.z - 0.5f) < epsilon) objectNormal = glm::vec3(0, 0, 1);
        else if (std::abs(objectHitPoint.z + 0.5f) < epsilon) objectNormal = glm::vec3(0, 0, -1);

        // Transform intersection point and normal to world space
        glm::vec3 worldHitPoint = Transform::transformPoint(objectHitPoint, transform.modelMatrix);
        glm::vec3 worldNormal = glm::normalize(Transform::transformDirection(objectNormal, transform.modelMatrix));
        
        float worldDist = glm::length(worldHitPoint - worldRay.getOrigin());
        return Intersection(worldDist, material, worldNormal);
    }

    glm::vec3 getDimensions() const { return dimensions; }
}; 