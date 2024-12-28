#pragma once
#include <glm/glm.hpp>

struct Material {
    glm::vec3 color;        // Base color of the material
    glm::vec3 ambient;      // Ambient reflection coefficient (per color channel)
    glm::vec3 diffuse;      // Diffuse reflection coefficient (per color channel)
    glm::vec3 specular;     // Specular reflection coefficient (per color channel)
    float shininess;        // Specular exponent (larger = smaller highlight)
    float reflectiveness;   // Global reflection coefficient (0 = no reflection, 1 = perfect mirror)
};