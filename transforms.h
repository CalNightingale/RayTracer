#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::mat4 modelMatrix;        // Object space to world space
    glm::mat4 viewMatrix;         // World space to camera space
    glm::mat4 projectionMatrix;   // Camera space to screen space
    
    Transform()
        : modelMatrix(1.0f)
        , viewMatrix(1.0f)
        , projectionMatrix(1.0f)
    {}

    // Transform a point from one space to another
    static glm::vec3 transformPoint(const glm::vec3& point, const glm::mat4& matrix) {
        glm::vec4 transformed = matrix * glm::vec4(point, 1.0f);
        return glm::vec3(transformed) / transformed.w;
    }

    // Transform a direction vector (ignores translation)
    static glm::vec3 transformDirection(const glm::vec3& dir, const glm::mat4& matrix) {
        return glm::vec3(matrix * glm::vec4(dir, 0.0f));
    }
}; 