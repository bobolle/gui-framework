#ifndef COMMON_H
#define COMMON_H

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

#endif
