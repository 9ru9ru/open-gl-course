cmake_minimum_required(VERSION 3.30)
project(OpenGLCourse)
set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCES "runtime/*.cpp" "runtime/*.h")

# include header files.
include_directories(include)

# link lib dir.
link_directories(lib)

# GLEW 정적 라이브러리 사용 시 필요한 정의 추가
add_definitions(-DGLEW_STATIC)

# build
add_executable(OpenGLCourse ${SOURCES})

# link libraries
set(OpenGLLibs glew32s glfw3dll opengl32)
target_link_libraries(OpenGLCourse ${OpenGLLibs})

# glfw3.dll을 실행 파일 폴더로 복사
add_custom_command(TARGET OpenGLCourse POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/glfw3.dll"
        "$<TARGET_FILE_DIR:OpenGLCourse>/glfw3.dll"
)