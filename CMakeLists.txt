cmake_minimum_required(VERSION 3.21)
project(C++_CA1_ImageManipulation)

set(CMAKE_CXX_STANDARD 17)

add_executable(C++_CA1 main.cpp
        Image.cpp
        Image.h
        Button.cpp
        Button.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/cmake/SFML)
find_package(SFML 3 COMPONENTS System Window Graphics REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SFML/include)
target_link_libraries(C++_CA1 PRIVATE SFML::System SFML::Window SFML::Graphics)

