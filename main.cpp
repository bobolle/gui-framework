extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "stdio.h"
#include "stdlib.h"
#include <GLFW/glfw3.h>

int main(void)
{
    int status, result, i;
    double sum;
    lua_State *L;

    // holds all lua contexts
    L = luaL_newstate();

    // load lua libs
    luaL_openlibs(L);

    // load lua script file
    status = luaL_loadfile(L, "script.lua");
    if (status)
    {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    // initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    // create window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
    if (!window)
    {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    // window loop
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    lua_close(L);

    return 0;
}
