#include "camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, float fov, float aspect)
    : fieldOfView(fov)
    , aspectRatio(aspect)
    , position(pos)
    , direction(glm::normalize(dir))
{
    updateViewMatrix();
    updateProjectionMatrix();
}

glm::vec3 Camera::getPosition() const { return position; }
glm::vec3 Camera::getDirection() const { return direction; }
float Camera::getFieldOfView() const { return fieldOfView; }

void Camera::setPosition(const glm::vec3& pos) { 
    position = pos; 
    updateViewMatrix();
}

void Camera::setDirection(const glm::vec3& dir) { 
    direction = glm::normalize(dir); 
    updateViewMatrix();
}

void Camera::setFieldOfView(float fov) { 
    fieldOfView = fov; 
    updateProjectionMatrix();
}

void Camera::move(const glm::vec3& offset) {
    position += offset;
    updateViewMatrix();
}

void Camera::lookAt(const glm::vec3& target) {
    direction = glm::normalize(target - position);
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    transform.viewMatrix = glm::lookAt(
        position,
        position + direction,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Camera::updateProjectionMatrix() {
    transform.projectionMatrix = glm::perspective(
        glm::radians(fieldOfView),
        aspectRatio,
        0.1f,
        100.0f
    );
}

Ray Camera::getRay(float x, float y) const {
    // Convert screen space to camera space
    glm::vec4 rayStart = glm::inverse(transform.projectionMatrix) * 
        glm::vec4(x * 2.0f - 1.0f, 1.0f - y * 2.0f, -1.0f, 1.0f);
    rayStart /= rayStart.w;
    
    // Convert to world space
    glm::mat4 invView = glm::inverse(transform.viewMatrix);
    glm::vec3 worldRayStart = Transform::transformPoint(glm::vec3(rayStart), invView);
    glm::vec3 worldRayDir = glm::normalize(worldRayStart - position);
    
    return Ray(position, worldRayDir);
} 