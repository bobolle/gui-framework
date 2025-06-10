#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Widget.hpp"

Widget::Widget(Cursor* cursor, unsigned int width, unsigned int height, Color color)
{
    this->color = color;
    this->bbox = {
        left: cursor->x,
        right: cursor->x + (int)width,
        top: cursor->y,
        bottom: cursor->y + (int)height
    };

    cursor->y += height;
}

Widget::~Widget()
{
}

PixelBBox Widget::getBounds()
{
    return this->bbox;
}

void Widget::render(unsigned int height)
{
    glClearColor(this->color.r, this->color.g, this->color.b, this->color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}
