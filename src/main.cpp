extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void renderText(const std::string& text, float x, float y)
{
    printf("%s\n", text);
}

int lua_renderText(lua_State* L)
{
    const std::string text = luaL_checkstring(L, 1);
    float x = static_cast<float>(luaL_checknumber(L, 2));
    float y = static_cast<float>(luaL_checknumber(L, 3));

    renderText(text, x, y);

    return 0;
}

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

    // register functions
    lua_register(L, "renderText", lua_renderText);

    // initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);

    if (!window)
    {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // window loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    lua_close(L);

    return 0;
}
