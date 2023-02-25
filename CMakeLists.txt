cmake_minimum_required(VERSION 3.23)
project(SoftRender)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC *.cpp)
add_executable(SoftRender ${SRC})
