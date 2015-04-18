#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>
#include <memory>
#include <map>
#include "gl_core_3_3.h"
#include "Resources/Texture.hpp"

namespace fumasu
{
	
class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	
	void Load(std::string identifier, std::string filename);
	Texture* Get(std::string identifier);

private:
	std::map<std::string, std::unique_ptr<Texture>> mTextures;
};
	
};

#endif