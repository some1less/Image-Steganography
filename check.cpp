#include <iostream>
#include <fstream>
#include <fmt/core.h>
#include <filesystem>
#include <string>
#include "check.h"
#include "algorithm"
#include <bitset>
#include <vector>

#include "imageDimension.h"

auto writeMessage(const std::string& message){
    auto message_to_write = std::string("[--MESSAGE--] " + message);
    return message_to_write;
}

auto messageToBits(const std::string& text) -> std::string {
    auto res = std::string();
    for (char c : text) {
        res += std::bitset<8>(c).to_string();
    }

    return res;
}

auto bitsToMessage(const std::string& bits) -> std::string {
    auto message = std::string();
    for (auto i = 0; i < bits.size(); i += 8) {
        auto byte = std::bitset<8>(bits.substr(i, 8));
        char c = static_cast<char>(byte.to_ulong());
        message+=c;
    }
    message.erase(std::remove(message.begin(), message.end(), '\0'), message.end());
    if (message.substr(0,14) == "[--MESSAGE--] "){
        return message;
    } else {
        message = std::string("[--ERROR--] You are trying to decode a parent image.");
        return message;
    }
}

auto supportedPath(const std::string& file) -> bool {
    auto fileCheck = std::fstream(file);
    if (!fileCheck){
        return false;
    }
    auto filePath = std::filesystem::path(file);

    auto extension = filePath.extension();
    if (extension == ".bmp" || extension == ".ppm") {
        return true;
    }

    if (extension.empty()) {
        auto bmpPath = filePath.string() + ".bmp";
        auto ppmPath = filePath.string() + ".ppm";
        if (std::filesystem::exists(bmpPath) || std::filesystem::exists(ppmPath)) {
            return true;
        }
    }

    return false;
}

auto checkIfFit(const std::string& filename, const char* message) -> bool {
    auto file = std::filesystem::path(filename);

    auto messageSize = strlen(message) * 8;

    if (file.extension() == ".bmp"){
        auto bmpImage = std::ifstream(file, std::ios::binary);
        char bmpHeader[54]; // 40 bytes info header + 14 bytes file header
        bmpImage.read(bmpHeader, 54);

        auto headerSize = *reinterpret_cast<int*>(&bmpHeader[14]);
        auto imagePixelSize = *reinterpret_cast<int*>(&bmpHeader[34]);

        if (messageSize + headerSize < imagePixelSize){
            fmt::println("[--MESSAGE--] : The string message can be fitted into image");
            return true;
        } else {
            fmt::println("[--ERROR--] : The string is too long.");
            return false;
        }
    } else if(file.extension() == ".ppm"){
        auto ppmImage = std::ifstream(file, std::ios::binary);

        std::string pSix;
        int width, height, maxColorValue;
        ppmImage >> pSix;
        ppmImage >> width >> height >> maxColorValue;
        ppmImage.ignore();
        auto headerSize = ppmImage.tellg();
        auto imagePixelSize = width * height * 3;

        ppmImage.close();

        if (messageSize + headerSize < imagePixelSize){
            fmt::println("[--MESSAGE--] : The string message can be fitted into image");
            return true;
        } else {
            fmt::println("[--ERROR--] : The string is too long.");
            return false;
        }

    }
    return false;
}

auto encodeMessage(const std::string& filename, const char* message) -> void {
    auto file = std::filesystem::path(filename);
    auto msgBits = messageToBits(writeMessage(message));
    auto messageSize = strlen(message) * 8;

    if (file.extension() == ".bmp"){
        auto bmpImage = std::ifstream(file, std::ios::binary);

        char bmpHeader[54];
        bmpImage.read(bmpHeader, 54);
        auto imagePixelSize = *reinterpret_cast<int*>(&bmpHeader[34]);
        auto headerSize     = *reinterpret_cast<int*>(&bmpHeader[14]);
        auto bitPerPixel    = *reinterpret_cast<int*>(&bmpHeader[28]);

        auto data = std::vector<char>(imagePixelSize);
        bmpImage.read(data.data(), data.size());

        if (messageSize + headerSize < imagePixelSize){
            auto cur_msg_bit = 0;

            for (auto i = 0; i < (int)data.size(); i += bitPerPixel) {
                data[i] &= 0b11111110;
                if (msgBits[cur_msg_bit] == '1') data[i] |= 1;
                cur_msg_bit++;
                if (cur_msg_bit >= (int)msgBits.size()) break;   // запобігаємо виходу за межі
            }

            // <-- ЗМІНА: пишемо в поточну теку (cmake-build-debug), тільки з іменем
            auto outName = std::string("enc_") + file.filename().string();
            std::ofstream readyFile(outName, std::ios::binary);
            if (!readyFile.is_open()){
                fmt::println("[--ERROR--] : Cannot open output file '{}'", outName);
                return;
            }

            readyFile.write(bmpHeader, 54);
            readyFile.write(data.data(), data.size());
            readyFile.close();
            fmt::println("[--MESSAGE--] : Your message was successfully encrypted to '{}'.", outName);
        } else {
            fmt::println("[--ERROR--] : The string is too long.");
        }
    } else if(file.extension() == ".ppm"){
        auto ppmImage = std::ifstream(file, std::ios::binary);

        std::string pSix;
        int width, height, maxColorValue;
        ppmImage >> pSix;
        ppmImage >> width >> height >> maxColorValue;
        ppmImage.ignore();
        auto headerSize     = ppmImage.tellg();
        auto imagePixelSize = width * height * 3;

        auto data = std::vector<char>(imagePixelSize);
        ppmImage.read(data.data(), imagePixelSize);
        ppmImage.close();

        if (messageSize + headerSize < imagePixelSize){
            auto cur_msg_bit = 0;

            for (auto i = 0; i < (int)data.size(); ++i){
                data[i] &= 0b11111110;
                if (msgBits[cur_msg_bit] == '1') data[i] |= 1;
                cur_msg_bit++;
                if (cur_msg_bit >= (int)msgBits.size()) break;   // запобігаємо виходу за межі
            }

            // <-- ЗМІНА: файл у поточній теці
            auto outName = std::string("enc_") + file.filename().string();
            std::ofstream readyFile(outName, std::ios::binary);
            if (!readyFile.is_open()){
                fmt::println("[--ERROR--] : Cannot open output file '{}'", outName);
                return;
            }
            readyFile << pSix << "\n" << width << " " << height << "\n" << maxColorValue << "\n";
            readyFile.write(data.data(), data.size());
            readyFile.close();

            fmt::println("[--MESSAGE--] : Your message was successfully encrypted to '{}'.", outName);
        } else {
            fmt::println("[--ERROR--] : The string is too long.");
        }
    } else {
        fmt::println("[--ERROR--] : You are trying to make an operation over unknown format.");
    }
}


auto decodeMessage(const std::string& filename) -> void {
    auto file = std::filesystem::path(filename);

    if (file.extension() == ".bmp"){
        auto bmpImage = std::ifstream(file, std::ios::binary);
        if (!bmpImage.is_open()) {
            fmt::println("[--ERROR--] : Could not open input file");
        }

        char BmpHeader[54]; // 40 bytes info header + 14 bytes file header
        bmpImage.read(BmpHeader, 54);
        auto imageSize = *reinterpret_cast<int *>(&BmpHeader[34]);
        auto bitPerPixel = *reinterpret_cast<int*>(&BmpHeader[28]);

        auto data = std::vector<char>(imageSize);
        bmpImage.read(data.data(), imageSize);
        bmpImage.close();

        auto msg_bits = std::string();
        for (auto i = 0; i < data.size(); i += bitPerPixel) {
            msg_bits += (data[i] & 1) ? '1' : '0';
        }

        fmt::println("{}", bitsToMessage(msg_bits));
    } else if (file.extension() == ".ppm"){
        auto ppmImage = std::ifstream(file, std::ios::binary);

        if (!ppmImage.is_open()) {
            fmt::println("[--ERROR--] : Could not open input file");
        }
        std::string pSix;
        int width, height, maxColorValue;
        ppmImage >> pSix;
        ppmImage >> width >> height >> maxColorValue;
        ppmImage.ignore();
        auto imagePixelSize = width * height * 3;

        auto data = std::vector<char>(imagePixelSize);
        ppmImage.read(data.data(), imagePixelSize);
        ppmImage.close();

        auto msg_bits = std::string();
        for (auto i = 0; i < data.size(); i++) {
            msg_bits += (data[i] & 1) ? '1' : '0';
        }

        fmt::println("{}", bitsToMessage(msg_bits));
    } else {
        fmt::println("[--ERROR--] : You are trying to make an operation over unknown format.");
    }
}