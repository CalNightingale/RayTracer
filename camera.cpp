#include "camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& dir, float fov, float aspect)
    : position(pos)
    , direction(glm::normalize(dir))
    , fieldOfView(fov)
    , aspectRatio(aspect)
{}

glm::vec3 Camera::getPosition() const { return position; }
glm::vec3 Camera::getDirection() const { return direction; }
float Camera::getFieldOfView() const { return fieldOfView; }

void Camera::setPosition(const glm::vec3& pos) { position = pos; }
void Camera::setDirection(const glm::vec3& dir) { direction = glm::normalize(dir); }
void Camera::setFieldOfView(float fov) { fieldOfView = fov; }

void Camera::move(const glm::vec3& offset) {
    position += offset;
}

void Camera::lookAt(const glm::vec3& target) {
    direction = glm::normalize(target - position);
}

Ray Camera::getRay(float x, float y) const {
    float theta = glm::radians(fieldOfView);
    float h = std::tan(theta/2);
    float viewport_height = 2.0f * h;
    float viewport_width = aspectRatio * viewport_height;

    // Calculate viewport vectors
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(direction, up));
    up = glm::cross(right, direction);

    glm::vec3 viewport_u = viewport_width * right;
    glm::vec3 viewport_v = viewport_height * -up;

    // Calculate the bottom-left corner of the viewport
    glm::vec3 viewport_corner = position + direction - viewport_u/2.0f + viewport_v/2.0f;

    // Calculate the ray direction for this pixel
    glm::vec3 rayDirection = viewport_corner + x*viewport_u - y*viewport_v - position;
    
    return Ray(position, rayDirection);
} 