#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include "gl_core_3_3.h"

namespace fumasu
{

class Texture
{
public:
	Texture();
	~Texture();
	
	bool LoadFromFile(std::string filename);
	GLuint GetHandle() const;
	
	const unsigned int GetWidth() const { return mWidth; }
	const unsigned int GetHeight() const { return mHeight; }
	const unsigned int GetBPP() const { return mBPP; }
	
private:
	GLuint mTextureId;
	unsigned int mWidth, mHeight, mBPP;
};

};

#endif