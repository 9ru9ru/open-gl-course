cmake_minimum_required(VERSION 3.30)
project(OpenGLCourse)
set(CMAKE_CXX_STANDARD 20)

# include header files.
include_directories(include)

# link lib dir.
link_directories(lib)

# build
add_executable(OpenGLCourse main.cpp)

# link libraries
set(OpenGLLibs glew32s glfw3 opengl32)
target_link_libraries(OpenGLCourse ${OpenGLLibs})