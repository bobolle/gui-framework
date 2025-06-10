#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
    private:
        void checkCompileErrors(unsigned int shader, std::string type);
    public:
        unsigned int ID;
        const char* vertexShaderSource = 
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec4 objectColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
            "}\n\0";
        Shader();
        void use();
};

#endif
