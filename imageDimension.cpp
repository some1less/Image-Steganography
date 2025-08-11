#include <iostream>
#include <fmt/core.h>
#include <fstream>
#include "imageDimension.h"

auto getBmpDimension(const std::string& filename, int& width, int& height) -> Dimensions {
    auto file = std::ifstream(filename, std::ios::binary);
    if (!file.is_open()){
        fmt::println("Error. Cannot find the file path. Try again");
    }

    char BmpHeader[54]; // 40 bytes info header + 14 bytes file header
    file.read(BmpHeader, 54);

    if (!file) {
        std::cerr << "Error while reading file." << std::endl;
    }

    width = *reinterpret_cast<int*>(&BmpHeader[18]);
    height = *reinterpret_cast<int*>(&BmpHeader[22]);
    file.close();

    return {width,height};

}

auto getPpmDimension(const std::string& filename,std::string& pSix, int& width, int& height) -> Dimensions {
    auto file = std::ifstream(filename, std::ios::binary);
    if (!file.is_open()){
        fmt::println("Error. Cannot find the file path. Try again");
    }
    file >> pSix >> width >> height;
    file.close();

    return {width, height};
}
