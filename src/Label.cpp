#include "Label.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Label::Label(Cursor* cursor, Shader* shader, GLuint* vbo, GLuint* vao, unsigned int width, unsigned int height, Color color, const std::string text, std::map<GLchar, Character>* characters) : Widget(cursor, shader, vbo, vao, width, height, color), text(text), characters(characters)
{
}

void Label::render(unsigned int height)
{
    PixelBBox bounds = this->getBounds();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(bounds.right - bounds.left), 0.0f, static_cast<float>(bounds.bottom - bounds.top));
    this->shader->use();
    glUniformMatrix4fv(glGetUniformLocation(this->shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(glGetUniformLocation(this->shader->ID, "textColor"), this->color.r, this->color.g, this->color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(*this->VAO);

    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
    for (auto c : this->text)
    {
        Character ch = (*this->characters)[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, *this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
