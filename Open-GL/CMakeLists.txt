cmake_minimum_required(VERSION 3.17)
project(Open_GL)

set(CMAKE_CXX_STANDARD 14)

#add_executable(Open-GL main.cpp)
#add_executable(Open-GL util.h demo.cpp)
add_executable(Open-GL util.h camera.cpp)
set(OpenGlLinkers -lglut -lGLU -lGL)
target_link_libraries(Open-GL ${OpenGlLinkers})