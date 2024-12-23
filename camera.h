#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"
#include "transforms.h"

class Camera {
private:
    Transform transform;
    float fieldOfView;
    float aspectRatio;
    glm::vec3 position;
    glm::vec3 direction;

public:
    Camera(const glm::vec3& pos = glm::vec3(0.0f),
           const glm::vec3& dir = glm::vec3(0.0f, 0.0f, -1.0f),
           float fov = 45.0f,
           float aspect = 16.0f/9.0f);

    // Getters
    glm::vec3 getPosition() const;
    glm::vec3 getDirection() const;
    float getFieldOfView() const;

    // Setters
    void setPosition(const glm::vec3& pos);
    void setDirection(const glm::vec3& dir);
    void setFieldOfView(float fov);

    // Movement and orientation
    void move(const glm::vec3& offset);
    void lookAt(const glm::vec3& target);

    // Matrix updates
    void updateViewMatrix();
    void updateProjectionMatrix();

    // Ray generation
    Ray getRay(float x, float y) const;
}; 