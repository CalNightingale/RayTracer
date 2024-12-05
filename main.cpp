#include <png.h>
#include <iostream>
#include <vector>

void write_png(const char* filename, int width, int height, std::vector<unsigned char>& data) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Failed to create PNG write struct." << std::endl;
        fclose(fp);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Failed to create PNG info struct." << std::endl;
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Error during PNG creation." << std::endl;
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);

    // Output PNG header
    png_set_IHDR(
        png,
        info,
        width,
        height,
        8,                     // Bit depth
        PNG_COLOR_TYPE_RGB,    // Color type
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // Write the image data row by row
    for (int y = 0; y < height; ++y) {
        png_write_row(png, &data[y * width * 3]);
    }

    // End write
    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    const int width = 50;
    const int height = 50;
    const int channels = 3; // RGB

    // Create a black image
    std::vector<unsigned char> image(width * height * channels, 0);

    const char* filename = "black_image.png";
    write_png(filename, width, height, image);

    std::cout << "Image saved successfully as '" << filename << "'." << std::endl;
    return 0;
}
