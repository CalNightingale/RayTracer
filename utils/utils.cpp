#include "utils.h"
#include <png.h>
#include <stdexcept>

bool write_png(const char* filename, int width, int height, const std::vector<unsigned char>& image) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return false;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return false;
    }

    png_init_io(png, fp);

    // Set image attributes
    png_set_IHDR(png, info, width, height, 8, 
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Allocate row pointers
    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = (png_bytep)&image[y * width * 3];
    }

    png_set_rows(png, info, row_pointers.data());
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return true;
} 