#include "Resources/Texture.hpp"

#include <iostream>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

namespace fumasu
{

Texture::Texture() : mTextureId{0}, mWidth{0}, mHeight{0}, mBPP{0}
{
}

Texture::~Texture()
{
	if (mTextureId !=0)
		glDeleteTextures(1, &mTextureId);
}

bool Texture::LoadFromFile(std::string filename)
{
	if (mTextureId ==0)
		glGenTextures(1, &mTextureId);

	ILuint texture;
	ilGenImages(1, &texture);
	ilBindImage(texture);
	
	if (!ilLoadImage(filename.c_str()))
	{
		glDeleteTextures(1, &mTextureId);
		mTextureId =0;
		ilDeleteImages(1, &texture);
		
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	mTextureId =ilutGLBindTexImage();
	
	mWidth =ilGetInteger(IL_IMAGE_WIDTH);
	mHeight =ilGetInteger(IL_IMAGE_HEIGHT);
	mBPP =ilGetInteger(IL_IMAGE_BPP);
	
	ilDeleteImages(1, &texture);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}

GLuint Texture::GetHandle() const
{
	return mTextureId;
}

};