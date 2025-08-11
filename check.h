#include <iostream>
#include <string>

auto supportedPath(const std::string& file) -> bool;
auto checkIfFit(const std::string& filename, const char* message) -> bool;
auto encodeMessage(const std::string& filename, const char* message) -> void;
auto decodeMessage(const std::string& filename) -> void;