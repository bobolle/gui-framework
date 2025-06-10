cmake_minimum_required(VERSION 3.16)
project(gui_framework)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(GLAD_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libs/glad/")
add_subdirectory("${GLAD_SOURCE_DIR}/cmake" glad_cmake)
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

file(GLOB LUA_SRC
    libs/lua/*.c
)

list(REMOVE_ITEM LUA_SRC
    "${CMAKE_SOURCE_DIR}/libs/lua/lua.c"
    "${CMAKE_SOURCE_DIR}/libs/lua/luac.c"
    "${CMAKE_SOURCE_DIR}/libs/lua/onelua.c"
)

add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC libs/lua)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(gui_framework
    src/main.cpp
    src/Shader.cpp
    src/Widget.cpp
    src/Layout.cpp
)

target_include_directories(gui_framework PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_include_directories(gui_framework PRIVATE "${GLAD_SOURCE_DIR}/include")

target_link_libraries(gui_framework
    PRIVATE
        OpenGL::GL
        glfw
        lua
        glad_gl_core_33
)
