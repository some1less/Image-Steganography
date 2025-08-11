#include <iostream>
#include <fmt/core.h>
#include <fstream>

struct Dimensions {
    int width;
    int height;
};

auto getBmpDimension(const std::string& filename, int& width, int& height) -> Dimensions;
auto getPpmDimension(const std::string& filename,std::string& pSix, int& width, int& height) -> Dimensions;
