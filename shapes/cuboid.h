#pragma once
#include "shape.h"
#include <algorithm>

class Cuboid : public Shape {
private:
    glm::vec3 dimensions;  // width, height, depth

public:
    Cuboid(const glm::vec3& pos, const glm::vec3& dims, const glm::vec3& col)
        : Shape(pos, col)
        , dimensions(dims)
    {}

    Intersection intersect(const Ray& worldRay) const override {
        // Transform ray to object space
        Ray objectRay = transformRayToObjectSpace(worldRay);

        // In object space, the cuboid is centered at origin with dimensions
        glm::vec3 min_bound = -dimensions * 0.5f;
        glm::vec3 max_bound = dimensions * 0.5f;

        glm::vec3 dirfrac = 1.0f / objectRay.getDirection();

        // Calculate intersection distances for each pair of slabs
        float t1 = (min_bound.x - objectRay.getOrigin().x) * dirfrac.x;
        float t2 = (max_bound.x - objectRay.getOrigin().x) * dirfrac.x;
        float t3 = (min_bound.y - objectRay.getOrigin().y) * dirfrac.y;
        float t4 = (max_bound.y - objectRay.getOrigin().y) * dirfrac.y;
        float t5 = (min_bound.z - objectRay.getOrigin().z) * dirfrac.z;
        float t6 = (max_bound.z - objectRay.getOrigin().z) * dirfrac.z;

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

        // Transform intersection point back to world space
        glm::vec3 objectHitPoint = objectRay.at(tmin);
        glm::vec3 worldHitPoint = Transform::transformPoint(objectHitPoint, transform.modelMatrix);
        float worldDist = glm::length(worldHitPoint - worldRay.getOrigin());

        return Intersection(worldDist, color);
    }

    glm::vec3 getDimensions() const { return dimensions; }
}; 