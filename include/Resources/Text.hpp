#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "gl_core_3_3.h"
#include <glm/glm.hpp>
#include "Resources/Texture.hpp"
#include "Resources/Shader.hpp"
#include "data_types.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_IDS_H

namespace priv
{

class Atlas
{
public:
	Atlas(size_t size);
	
	bool InsertImage(std::wstring identifier, size_t width, size_t height, void* data);
	glm::vec4 GetImage(std::wstring identifier);
	const size_t GetAtlasSize() const
	{
		return mSize;
	}
	const std::vector<unsigned char>& GetImageData() const
	{
		return mImageData;
	}

private:
	std::vector<unsigned char> mImageData;
	std::map<std::wstring, glm::vec4> mImageIndex;
	size_t mMaxHeight;
	size_t mTop, mLeft;
	size_t mSize;
};

};

namespace fumasu
{

class Context;

class Text
{
public:
	using Ptr =std::unique_ptr<Text>;
	Text(Context* ctx);
	~Text();
	
	void LoadFromFile(std::string filename, size_t size);
	void DrawText(std::wstring text, glm::vec2 position, glm::vec3 textColor =glm::vec3(1.f, 1.f, 1.f));
	
	void GetTextSize(const std::wstring text, glm::vec2& size);
	glm::vec2 GetTextSize(const std::wstring text);
	
private:
	struct Glyph
	{
		std::vector<unsigned char> data;
		size_t width;
		size_t height;
		int left;
		int top;
		struct
		{
			int x;
			int y;
		}advance;
		size_t debug;
	};
	
	void addCharToAtlas(wchar_t ch);
	
	FT_Library mLibrary;
	FT_Face mFace;
	std::map<wchar_t, Glyph> mGlyphs;
	priv::Atlas mAtlas;
	
	Shader mShader;
	GLuint mVAO, mVBO, mTexture;
	std::vector<Vertex> mVertices;
	glm::mat4 mProjection;
	size_t mSize;
	Context* mContext;
};

};

#endif