#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
    private:
        void checkCompileErrors(unsigned int shader, std::string type);
    public:
        unsigned int ID;
        const char* vertexShaderSource;
        const char* fragmentShaderSource;
        Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        void use();
};

#endif
