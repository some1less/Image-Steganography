#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include "imageDimension.h"
#include <chrono>
#include <filesystem>
#include "print.h"

auto pathPrint(const std::string& file) -> void {
    auto filepath = std::fstream(file);
    if(!filepath){
        fmt::println("Error");
    }
    auto line = std::string();

    while(std::getline(filepath, line)){
        fmt::println("{}", line);
    }
}

auto printInfo(const std::string& file) -> void {
    auto filePath = std::filesystem::path(file);

    auto extension = filePath.extension();
    if (extension == ".bmp") {

        int width, height;
        getBmpDimension(file, width, height);

        auto timestamp = std::chrono::file_clock::to_sys(
                std::filesystem::last_write_time(filePath)
        );

        auto memoryUse = std::filesystem::file_size(filePath);

        auto text = fmt::format(
                "\nFile format: {}\nImage size: {}x{}\nMemory usage: {} bytes\nLast modification timestamp: {}",
                extension.string(), width, height, memoryUse, timestamp);

        fmt::println("{}", text);
    }

    if (extension == ".ppm") {

        std::string pSix;
        int width, height;
        getPpmDimension(file, pSix, width, height);

        auto timestamp = std::chrono::file_clock::to_sys(
                std::filesystem::last_write_time(filePath)
        );

        auto memoryUse = std::filesystem::file_size(filePath);

        auto text = fmt::format(
                "\nFile format: {}\nImage size: {}x{}\nMemory usage: {} bytes\nLast modification timestamp: {}",
                extension.string(), width, height, memoryUse, timestamp);

        fmt::println("{}", text);

    }

    if (extension.empty()) {
//        auto bmpPath = filePath.string() + ".bmp";
//        if (std::filesystem::exists(bmpPath)){
//
//            auto filePath = std::filesystem::path(bmpPath);
//
//            auto extension = filePath.extension();
//
//            int width, height;
//            getBmpDimension(bmpPath,width,height);
//
//            auto timestamp = std::chrono::file_clock::to_sys(
//                    std::filesystem::last_write_time(bmpPath)
//            );
//
//            auto memoryUse = std::filesystem::file_size(bmpPath);
//
//            auto text = fmt::format("\nFile format: {}\nImage size: {}x{}\nMemory usage: {} bytes\nLast modification timestamp: {}",
//                                    extension.string(),width,height,memoryUse,timestamp);
//
//            fmt::println("{}", text);
//        }
//
//        auto ppmPath = filePath.string() + ".ppm";
//        if(std::filesystem::exists(ppmPath)) {
//
//            auto filePath = std::filesystem::path(ppmPath);
//
//            auto extension = filePath.extension();
//
//            std::string pSix;
//            int width, height;
//            getPpmDimension(ppmPath, pSix, width, height);
//
//            auto timestamp = std::chrono::file_clock::to_sys(
//                    std::filesystem::last_write_time(ppmPath)
//            );
//
//            auto memoryUse = std::filesystem::file_size(ppmPath);
//
//            auto text = fmt::format("\nFile format: {}\nImage size: {}x{}\nMemory usage: {} bytes\nLast modification timestamp: {}",
//                                    extension.string(),width,height,memoryUse,timestamp);
//
//            fmt::println("{}", text);
//        }
//    }
        fmt::println("Cannot determine the format of image, please add extension to the file");
    }
}

