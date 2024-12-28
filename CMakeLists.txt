cmake_minimum_required(VERSION 3.10)
project(raytracer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(PNG REQUIRED)
find_package(GLM REQUIRED)

# Include directories
include_directories(${PNG_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

# Add the executable
add_executable(raytracer 
    main.cpp 
    camera.cpp 
    scene.cpp
    utils/utils.cpp
)

# Link libraries
target_link_libraries(raytracer 
    ${PNG_LIBRARY}
)
