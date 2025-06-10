#include "Layout.hpp"
#include "Widget.hpp"

#include <glad/gl.h>

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
