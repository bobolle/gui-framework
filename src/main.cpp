extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Widget.hpp"
#include "Layout.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //std::cout << "mouse left pressed\n";
    }
}

static GLFWwindow* initWindow()
{
    GLFWwindow* window;
    // initialize GLFW
    if (!glfwInit())
    {
        std::cout <<  "Failed to initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    // event callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context\n";
    }

    return window;
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

class Rectangle : public Widget
{
        using Widget::Widget;
    public:
        void render(unsigned int height) override;
};

void Rectangle::render(unsigned int height)
{
    glClearColor(this->color.r, this->color.g, this->color.b, this->color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

class Triangle : public Widget
{
        using Widget::Widget;
    public:
        void render(unsigned int height) override;
};

void Triangle::render(unsigned int height)
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void)
{
    int status;
    lua_State *L;

    // holds all lua contexts
    L = luaL_newstate();

    // load lua libs
    luaL_openlibs(L);

    // load lua script file
    status = luaL_loadfile(L, "script.lua");
    if (status)
    {
        std::cout << "Couldn't load file: " << lua_tostring(L, -1) << std::endl;
        exit(1);
    }

    // register functions
    lua_register(L, "renderText", lua_renderText);

    // create window
    GLFWwindow* window = initWindow();

    // vertices
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // left
         1.0f, -1.0f, 0.0f, // right
         0.0f,  1.0f, 0.0f,  // top
    };

    // create Shader Program
    Shader shader;

    // Vertex Array Object and Vertex Buffer Object
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // copy vertices to buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex data interpretation
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindVertexArray(0);

    glEnable(GL_SCISSOR_TEST);

    // window loop
    InputState input_state;
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        // render
        glScissor(0, 0, width, height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        Layout layout(&input_state);

        Rectangle rec(&layout.cursor, 100, 100, {r: 0.3f, g: 0.5f, b: 0.2f, a: 1.0f});
        Triangle tri(&layout.cursor, 100, 100, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f});
        Triangle tri2(&layout.cursor, 100, 100, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f});

        layout.pushWidget(&rec);
        layout.pushWidget(&tri);
        layout.pushWidget(&tri2);

        layout.handleInput(window);
        layout.render(height);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    lua_close(L);

    return 0;
}
