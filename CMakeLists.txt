cmake_minimum_required(VERSION 3.22)
project(meme_maker)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET "gtk+-2.0")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(meme_maker main.cpp MainWindow.cpp MainWindow.h)
target_link_libraries(meme_maker PRIVATE PkgConfig::GTK ${OpenCV_LIBS})