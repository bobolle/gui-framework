extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

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

static void renderText(const std::string& text, float x, float y)
{
    printf("%s\n", text);
}

static GLFWwindow* initWindow()
{
    GLFWwindow* window;
    // initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
    if (!window)
    {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
    }

    return window;
}

int lua_renderText(lua_State* L)
{
    const std::string text = luaL_checkstring(L, 1);
    float x = static_cast<float>(luaL_checknumber(L, 2));
    float y = static_cast<float>(luaL_checknumber(L, 3));

    renderText(text, x, y);

    return 0;
}

class Cursor {
    public:
        int x;
        int y;
        Cursor()
        {
            x = 0;
            y = 0;
        }
};

struct Color {
    float r;
    float g;
    float b;
    float a;
};

class Rectangle {
    public:
        int pos_x;
        int pos_y;
        unsigned int width;
        unsigned int height;
        Color color;
        Rectangle(Cursor* cursor, unsigned int width, unsigned int height, Color color);
        void render(unsigned int height);
};

Rectangle::Rectangle(Cursor* cursor, unsigned int width, unsigned int height, Color color)
{
    this->pos_x = cursor->x;
    this->pos_y = cursor->y;
    this->width = width;
    this->height = height;

    this->color = color;

    cursor->y += height;
}

void Rectangle::render(unsigned int height)
{
    glScissor(this->pos_x, height - this->pos_y - this->height, this->width, this->height);
    glViewport(this->pos_x, height - this->pos_y - this->height, this->width, this->height);
    glClearColor(this->color.r, this->color.g, this->color.b, this->color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

class Triangle {
    public:
        unsigned int* VAO;
        int pos_x;
        int pos_y;
        unsigned int width;
        unsigned int height;
        Color color;
        Triangle(Cursor* cursor, unsigned int* VAO, unsigned int width, unsigned int height, Color color);
        void render(unsigned int height);
};

Triangle::Triangle(Cursor* cursor, unsigned int* VAO, unsigned int width, unsigned int height, Color color)
{
    this->pos_x = cursor->x;
    this->pos_y = cursor->y;
    this->width = width;
    this->height = height;

    this->color = color;
    this->VAO = VAO;

    cursor->y += height;
}

void Triangle::render(unsigned int height)
{
    // do stuff
    glScissor(this->pos_x, height - this->pos_y - this->height, this->width, this->height);
    glViewport(this->pos_x, height - this->pos_y - this->height, this->width, this->height);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        // render
        glScissor(0, 0, width, height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Cursor cursor;
        Rectangle rec(&cursor, 100, 100, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f});
        Rectangle rec2(&cursor, 200, 200, {r: 1.0f, g: 1.0f, b: 0.0f, a: 1.0f});
        rec.render(height);
        rec2.render(height);

        shader.use();
        glBindVertexArray(VAO);
        Triangle tri(&cursor, &VAO, 100, 100, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f});
        tri.render(height);
        Triangle tri2(&cursor, &VAO, 100, 100, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f});
        tri2.render(height);
        
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
