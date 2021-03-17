cmake_minimum_required(VERSION 3.17)
project(Open_GL)

set(CMAKE_CXX_STANDARD 11)

#add_executable(Open-GL demo2.cpp)
#add_executable(Open-GL template.cpp)
#add_executable(Open-GL point.h functions.h problem1.cpp)
add_executable(Open-GL point.h functions.h problem2.cpp)
set(OpenGlLinkers -lglut -lGLU -lGL)
target_link_libraries(Open-GL ${OpenGlLinkers})