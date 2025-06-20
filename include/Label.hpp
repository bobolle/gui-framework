#ifndef LABEL_H
#define LABEL_H

#include "Widget.hpp"

#include <map>
#include <glm/glm.hpp>

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class Label : public Widget {
    using Widget::Widget;
    public:
        std::map<GLchar, Character>* characters;
        std::string text;
        Label(Cursor* cursor, Shader* shader, GLuint* vbo, GLuint* vao, unsigned int width, unsigned int height, Color color, const std::string text, std::map<GLchar, Character>* characters);
        void render(unsigned int height) override;
};

#endif
