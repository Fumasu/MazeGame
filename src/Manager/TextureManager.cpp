#include "Manager/TextureManager.hpp"

#include <iostream>
#include <cassert>

#include <IL/il.h>
#include <IL/ilut.h>

namespace fumasu
{

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::Load(std::string identifier, std::string filename)
{
	auto found =mTextures.find(identifier);
	if (found !=mTextures.end())	//Texture already exists? do nothing
		return;
	
	std::unique_ptr<Texture> texture(new Texture);
	if (!texture->LoadFromFile(filename))
	{
		std::cout <<"Unable to load the Image: " <<filename <<"\n";
		return;
	}
	
	mTextures[identifier] =std::move(texture);
}

Texture* TextureManager::Get(std::string identifier)
{
	auto found =mTextures.find(identifier);
	assert (found !=mTextures.end());
	
	if (found ==mTextures.end())
		return nullptr;
		
	return mTextures[identifier].get();
}

};