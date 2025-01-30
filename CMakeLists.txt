cmake_minimum_required(VERSION 3.30)
project(OpenGLCourse)
set(CMAKE_CXX_STANDARD 20)

link_directories(lib)
set(OpenGLLibs glew32s glfw3 opengl32)

include_directories(include)

add_executable(OpenGLCourse main.cpp)

target_link_libraries(OpenGLCourse ${OpenGLLibs})