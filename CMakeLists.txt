cmake_minimum_required(VERSION 3.27)
project(ImageSteganography LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(ImageSteganography main.cpp
        print.h
        launch.h
        check.h
        imageDimension.h
        launch.cpp
        imageDimension.cpp
        print.cpp
        check.cpp)

add_custom_command(TARGET ImageSteganography POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/content"
        "$<TARGET_FILE_DIR:ImageSteganography>/content")

target_link_libraries(ImageSteganography fmt)