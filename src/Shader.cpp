#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader() : mProgram (0)
{
}

Shader::~Shader()
{
    if (mProgram !=0)
    {
        glDeleteProgram (mProgram);
        mProgram =0;
    }
}

bool Shader::loadFromFile(const std::string vertexShader, const std::string fragmentShader)
{
    if (mProgram !=0)
    {
        glDeleteProgram (mProgram);
        mProgram =0;
    }
        
    std::string vertexCode, fragmentCode;
    std::ifstream vertexFile, fragmentFile;
    
    vertexFile.exceptions (std::ifstream::badbit);
    fragmentFile.exceptions (std::ifstream::badbit);
    
    try {
        std::stringstream vertexStream, fragmentStream;
        
        vertexFile.open (vertexShader);
        fragmentFile.open (fragmentShader);
        
        vertexStream <<vertexFile.rdbuf();
        fragmentStream <<fragmentFile.rdbuf();
        
        vertexFile.close();
        fragmentFile.close();
        
        vertexCode =vertexStream.str();
        fragmentCode =fragmentStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr <<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        
        return false;
    }
    
    const GLchar* glvertexCode =vertexCode.c_str();
    const GLchar* glfragmentCode =fragmentCode.c_str();
    
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    
    vertex =glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertex, 1, &glvertexCode, nullptr);
    glCompileShader (vertex);
    glGetShaderiv (vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog (vertex, 512, nullptr, infoLog);
        std::cerr <<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<infoLog <<"\n";
        
        return false;
    }
    
    fragment =glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fragment, 1, &glfragmentCode, nullptr);
    glCompileShader (fragment);
    glGetShaderiv (fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog (fragment, 512, nullptr, infoLog);
        std::cerr <<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<infoLog <<"\n";
        
        return false;
    }
    
    mProgram =glCreateProgram ();
    glAttachShader (mProgram, vertex);
    glAttachShader (mProgram, fragment);
    glLinkProgram (mProgram);
    
    glGetProgramiv (mProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog (mProgram, 512, nullptr, infoLog);
        std::cerr <<"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<infoLog <<"\n";
        
        return false;
    }
    
    glDeleteShader (vertex);
    glDeleteShader (fragment);
    
    return true;
}

void Shader::bind() const
{
    glUseProgram (getNativeHandle());
}

GLuint Shader::getNativeHandle() const
{
    return (mProgram ? mProgram : 0);
}