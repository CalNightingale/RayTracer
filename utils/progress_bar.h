#pragma once
#include <iostream>
#include <string>

class ProgressBar {
private:
    int width;
    int total;
    std::string prefix;

public:
    ProgressBar(int total_steps, int bar_width = 50, const std::string& prefix_text = "Rendering: ")
        : width(bar_width), total(total_steps), prefix(prefix_text) {}

    void update(int current) {
        float progress = static_cast<float>(current) / total;
        int pos = static_cast<int>(width * progress);

        std::cout << "\r" << prefix;
        std::cout << "[";
        for (int i = 0; i < width; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << "% ";
        std::cout.flush();

        if (current == total) {
            std::cout << std::endl;
        }
    }
}; 