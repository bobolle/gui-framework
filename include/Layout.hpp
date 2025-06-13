#ifndef LAYOUT_H
#define LAYOUT_H

#include "Common.hpp"
#include "Widget.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class InputState {
    public:
        bool left_mouse_click;
        bool right_mouse_click;
        InputState();
};

class Layout {
    public:
        Cursor cursor;
        InputState* input_state;
        std::vector<Widget*> widgets;
        Layout(InputState* input_state);
        void pushWidget(Widget* widget);
        void render(unsigned int height);
        void handleInput(GLFWwindow* window);
};

#endif
