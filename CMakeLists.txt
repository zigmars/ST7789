cmake_minimum_required(VERSION 3.1...3.22)

project(st7789
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(st7789 src/st7789.cpp)

target_include_directories(st7789 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
