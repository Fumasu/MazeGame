#include "Texture.hpp"

#include <iostream>

#include <SOIL/SOIL.h>

Texture::Texture() : mID(0)
{
}

Texture::~Texture ()
{
    if (mID !=0)
    {
        glDeleteTextures (1, &mID);
        mID =0;
    }
}

bool Texture::loadFromFile (const std::string filename)
{
    if (mID !=0)
    {
        glDeleteTextures (1, &mID);
        mID =0;
    }
    
    int width, height;
    unsigned char* image =SOIL_load_image (filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    
    if (image ==nullptr)
    {
        std::cerr <<"ERROR:TEXTURE::LOADING_FAILED\n" <<SOIL_last_result() <<"\n";
        
        return false;
    }
    
    glGenTextures (1, &mID);
    glBindTexture (GL_TEXTURE_2D, mID);
    
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap (GL_TEXTURE_2D);
    
    SOIL_free_image_data (image);
    glBindTexture (GL_TEXTURE_2D, 0);
    
    return true;
}

void Texture::bind() const
{
    glBindTexture (GL_TEXTURE_2D, (mID ? mID : 0));
}