#include "Layout.hpp"
#include "Widget.hpp"

#include <iostream>

InputState::InputState()
{
    this->left_mouse_click = false;
    this->right_mouse_click = false;
}

Layout::Layout(InputState* input_state)
{
    this->input_state = input_state;
}

void Layout::pushWidget(Widget* widget)
{
    this->widgets.push_back(widget);
}

void Layout::render(unsigned int height)
{
    for (Widget* widget : this->widgets)
    {
        PixelBBox bounds = widget->getBounds();
        glScissor(bounds.left, height - bounds.bottom, bounds.right - bounds.left, bounds.bottom - bounds.top);
        glViewport(bounds.left, height - bounds.bottom, bounds.right - bounds.left, bounds.bottom - bounds.top);
        widget->render(height);
    }
}

void Layout::handleInput(GLFWwindow* window)
{
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS && this->input_state->left_mouse_click == false)
    {
        this->input_state->left_mouse_click = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        for (Widget* widget : this->widgets)
        {
            PixelBBox bounds = widget->getBounds();
            if ((xpos > bounds.left && xpos < bounds.right) && (ypos > bounds.top && ypos < bounds.bottom))
            {
                widget->color.r = 0;
            }
        }
    } else
    if (state == GLFW_RELEASE && this->input_state->left_mouse_click == true)
    {
        this->input_state->left_mouse_click = false;
    }
}
