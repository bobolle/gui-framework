#ifndef WIDGET_H
#define WIDGET_H

#include "Common.hpp"
#include "Shader.hpp"

#include <glad/gl.h>

class Widget {
    public:
        Shader* shader;
        GLuint* VBO;
        GLuint* VAO;
        PixelBBox bbox;
        Color color;
        Widget(Cursor* cursor, Shader* shader, GLuint* vbo, GLuint* vao, unsigned int width, unsigned int height, Color color);
        ~Widget();
        PixelBBox getBounds();
        virtual void render(unsigned int height);
};

#endif
