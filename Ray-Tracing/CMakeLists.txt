cmake_minimum_required(VERSION 3.17)
project(Ray_Tracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ray-Tracing functions.h main.cpp)
set(OpenGlLinkers -lglut -lGLU -lGL)
target_link_libraries(Ray-Tracing ${OpenGlLinkers})