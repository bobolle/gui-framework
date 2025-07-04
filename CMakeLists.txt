cmake_minimum_required(VERSION 3.16)
project(gui_framework)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE debug)

find_package(OpenGL REQUIRED)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/freetype)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glm)

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
    src/Label.cpp
)

target_include_directories(gui_framework PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_include_directories(gui_framework PRIVATE "${GLAD_SOURCE_DIR}/include")

target_link_libraries(gui_framework
    PRIVATE
        lua
        OpenGL::GL
        glfw
        glad_gl_core_33
        freetype
        glm
)
