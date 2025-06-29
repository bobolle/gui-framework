extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Widget.hpp"
#include "Layout.hpp"
#include "Label.hpp"

std::map<GLchar, Character> Characters;
GLuint tVAO, tVBO;
GLuint VAO, VBO;

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

static void init_fft()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(1);
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/Monaco_Linux.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 14);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

class Rectangle : public Widget {
        using Widget::Widget;
    public:
        void render(unsigned int height) override;
};

void Rectangle::render(unsigned int height)
{
    this->shader->use();
    glClearColor(this->color.r, this->color.g, this->color.b, this->color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

class Triangle : public Widget {
        using Widget::Widget;
    public:
        void render(unsigned int height) override;
};

void Triangle::render(unsigned int height)
{
    this->shader->use();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int luaFunction(lua_State* L)
{
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    lua_pushnumber(L, a + b);
    return 1;
}

int main(void)
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "cadd", luaFunction);

    if (luaL_dofile(L, "script.lua") != LUA_OK)
    {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
    }

    lua_close(L);

    // create window
    GLFWwindow* window = initWindow();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init_fft();

    // create Shader Program
    const char* vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 objectColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n\0";
    Shader shader(vertexShaderSource, fragmentShaderSource);

    const char* tvertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "    TexCoords = vertex.zw;\n"
        "}\0";
        const char* tfragmentShaderSource =
        "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 color;\n"
        "uniform sampler2D text;\n"
        "uniform vec3 textColor;\n"
        "void main()\n"
        "{\n"
        "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
        "    color = vec4(textColor, 1.0) * sampled;\n"
        "}\0";
    Shader text_shader(tvertexShaderSource, tfragmentShaderSource);


    glGenVertexArrays(1, &tVAO);
    glGenBuffers(1, &tVBO);
    glBindVertexArray(tVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // vertices
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // left
         1.0f, -1.0f, 0.0f, // right
         0.0f,  1.0f, 0.0f,  // top
    };

    // Vertex Array Object and Vertex Buffer Object
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

        glBindVertexArray(VAO);

        Layout layout(&input_state);

        Rectangle rec(&layout.cursor, &shader, &VBO, &VAO, 100, 100, {r: 0.3f, g: 0.5f, b: 0.2f, a: 1.0f});
        Triangle tri(&layout.cursor, &shader, &VBO, &VAO, 100, 100, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f});
        Label lab(&layout.cursor, &text_shader, &tVBO, &tVAO, 300, 30, {r: 1.0f, g: 0.5f, b: 0.2f, a: 1.0f}, "this is a label", &Characters);

        layout.pushWidget(&rec);
        layout.pushWidget(&tri);
        layout.pushWidget(&lab);

        layout.handleInput(window);
        layout.render(height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &tVAO);
    glDeleteBuffers(1, &tVBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
