#include <iostream>
#include <fmt/core.h>
#include "print.h"
#include "check.h"
#include "launch.h"

namespace fs = std::filesystem;

static std::optional<std::string> normalizeImagePath(const std::string& file) {
    fs::path p(file);

    if (exists(p)) return p.string();

    if (p.extension().empty()) {
        fs::path bmp = p; bmp += ".bmp";
        if (exists(bmp)) return bmp.string();
        fs::path ppm = p; ppm += ".ppm";
        if (exists(ppm)) return ppm.string();
    }
    return std::nullopt;
}

auto baseFunc(int argc, char* argv[]) -> void {
    auto args = std::vector<std::string>(argv, argv + argc);

    if (args.size() == 1) {
        fmt::println("Not enough arguments. Please, go through command '-h' or '--help' to learn more about this program");
        return;
    }

    auto exeDir   = std::filesystem::path(argv[0]).parent_path();
    auto helpPath = exeDir / "content" / "help.txt";

    if (args[1] == "-h" || args[1] == "--help") {              /*HELP <-----------*/
        if (argc == 2) {
            pathPrint(helpPath.string());
        } else {
            fmt::println("{} does not need arguments", argv[1]);
        }
    } else if (args[1] == "-i" || args[1] == "--info") {           /*INFO <-----------*/
        if (args.size() == 3) {

            if (auto p = normalizeImagePath(args[2])) {
                fmt::println("[--FILE INFO--]");
                printInfo(*p);
            } else {
                fmt::println("[--ERROR--] Cannot find the file.");
            }

        } else {
            fmt::println("{} needs only 1 argument", argv[1]);
        }

    } else if (args[1] == "-d" || args[1] == "--decrypt") {     /*DECODING <-----------*/
        if (args.size() == 3) {

            if (auto p = normalizeImagePath(args[2])) {
                decodeMessage(*p);
            } else {
                fmt::println("[--ERROR--] Cannot find the file.");
            }

        } else {
            fmt::println("{} needs only 1 argument", argv[1]);
        }
    } else if (args[1] == "-e" || args[1] == "--encrypt") {       /*ENCODING <-----------*/
        if (args.size() == 4) {

            auto text = argv[3];
            if (auto p = normalizeImagePath(args[2])) {
                encodeMessage(*p, text);
            } else {
                fmt::println("[--ERROR--] Cannot find the file.");
            }

        } else {
            fmt::println("{} needs only 2 arguments", argv[1]);
        }
    } else if (args[1] == "-c" || args[1] == "--check") {           /*CHECKING <-----------*/
        if (args.size() == 4) {

            auto text = argv[3];
            if (auto p = normalizeImagePath(args[2])) {
                checkIfFit(*p, text);
            } else {
                fmt::println("[--ERROR--] Cannot find the file.");
            }
        } else {
            fmt::println("{} needs only 2 arguments", argv[1]);
        }
    } else {
        fmt::println("Unknown command. Please, read the info about program by '-h' or '--help'.");
    }
}