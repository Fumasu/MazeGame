#pragma once

#include <string>

#include "gl_core_3_3.h"

class Texture
{
public:
    Texture();
    ~Texture();

    bool loadFromFile (const std::string filename);
    void bind () const;
    
private:
    GLuint mID;
};

