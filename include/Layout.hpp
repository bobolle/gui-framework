#ifndef LAYOUT_H
#define LAYOUT_H

#include "Widget.hpp"

#include <vector>

class Layout {
    public:
        Cursor cursor;
        std::vector<Widget*> widgets;
        void pushWidget(Widget* widget);
        void render(unsigned int height);
};

#endif
