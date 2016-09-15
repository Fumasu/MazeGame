#pragma once

#include <string>

#include "gl_core_3_3.h"

class Shader
{
public:
    Shader();
    ~Shader();
    
    bool loadFromFile (const std::string vertexShader, const std::string fragmentShader);
    
    void bind() const;
    GLuint getNativeHandle () const;
    
private:
    GLuint mProgram;
};

