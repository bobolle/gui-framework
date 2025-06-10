#ifndef WIDGET_H
#define WIDGET_H

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

struct PixelBBox {
    int left;
    int right;
    int top;
    int bottom;
};

struct Color {
    float r;
    float g;
    float b;
    float a;
};

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
