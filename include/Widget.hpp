#ifndef WIDGET_H
#define WIDGET_H

#include "Common.hpp"

class Widget {
    public:
        PixelBBox bbox;
        Color color;
        Widget(Cursor* cursor, unsigned int width, unsigned int height, Color color);
        ~Widget();
        PixelBBox getBounds();
        virtual void render(unsigned int height);
};

#endif
