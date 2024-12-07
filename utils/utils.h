#pragma once
#include <vector>
#include <string>

// Write image data to a PNG file
bool write_png(const char* filename, int width, int height, const std::vector<unsigned char>& image);
