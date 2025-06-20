#include "Shader.hpp"
#include <string>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    unsigned int vertexShader;
    unsigned int fragmentShader;

    // VERTEX SHADER
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    this->checkCompileErrors(vertexShader, "VERTEX");

    // FRAGMENT SHADER
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    this->checkCompileErrors(fragmentShader, "FRAGMENT");

    // SHADER PROGRAM
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);

    this->checkCompileErrors(this->ID, "PROGRAM");

    // cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
};

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Shader::use()
{
    glUseProgram(this->ID);
}
