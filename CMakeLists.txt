cmake_minimum_required(VERSION 3.16)
project(eldenring_sc)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/glfw)

include_directories(libs/glfw/include)

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

add_executable(eldenring_sc src/main.cpp)

target_link_libraries(eldenring_sc glfw lua)
