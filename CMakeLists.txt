cmake_minimum_required(VERSION 3.10)
project(raytracer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find libpng
find_package(PNG REQUIRED)

# Include directories
include_directories(${PNG_INCLUDE_DIR})

# Add the executable
add_executable(raytracer main.cpp)

# Link libpng
target_link_libraries(raytracer ${PNG_LIBRARY})
