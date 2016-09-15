#include "Resources/Text.hpp"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Context.hpp"

namespace priv{

Atlas::Atlas(size_t size) : mMaxHeight{0}, mTop{0}, mLeft{0}, mSize{size}
{
	mImageData.resize(size * size);
}

bool Atlas::InsertImage(std::wstring identifier, size_t width, size_t height, void* data)
{
	auto found =mImageIndex.find(identifier);
	if (found !=mImageIndex.end())	//wenn das Image bereits hinzugefügt wurde, dann abbrechen
		return true;
	
	if (mLeft + width > mSize)
	{
		if (mTop + mMaxHeight + height >mSize)	//Atlas zu klein, um das image hinzuzufügen
			return false;
		
		mTop +=mMaxHeight;
		mLeft =0;
		mMaxHeight =0;
	}
	
	for (size_t y =0;y <height;++y)
	{
		for (size_t x =0;x <width;++x)
		{
			mImageData[(mLeft +x ) + ((mTop + y) * mSize)] =static_cast<unsigned char*>(data)[x + y * width];
		}
	}
	
	glm::vec4 position(mLeft, mTop, width, height);
	mImageIndex.emplace(identifier, position);
	
	mMaxHeight =std::max(mMaxHeight, height);
	mLeft +=width;
	
	return true;
}

glm::vec4 Atlas::GetImage(std::wstring identifier)
{
	auto found =mImageIndex.find(identifier);

	if (found ==mImageIndex.end())
	{
		return glm::vec4(-1.f);
	}
	
	return mImageIndex.at(identifier);
}

};

namespace fumasu
{

Text::Text(Context* ctx) : mAtlas{1024}, mProjection{1.f}, mContext{ctx}
{
	mShader.LoadFromFile(Shader::VertexShader, "res/Shader/Text.vert");
	mShader.LoadFromFile(Shader::FragmentShader, "res/Shader/Text.frag");
	mShader.UseShader();
	
	std::cerr <<mShader.GetErrorLog();
	
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glBindVertexArray(0);
	
	size_t width, height;
	mContext->GetGameSize(width, height);
	mProjection =glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f);
	
	glActiveTexture(0);
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Text::~Text()
{
	glDeleteTextures(1,&mTexture);
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
	
	FT_Done_Face(mFace);
	FT_Done_FreeType(mLibrary);
}

void Text::LoadFromFile(std::string filename, size_t size)
{
	FT_Library library;
	FT_Face face;
	
	mSize =size;
	
	if (FT_Init_FreeType(&library))
	{
		std::cerr <<"Error opening the FreeType Library\n";
	}
	mLibrary =library;
	if (FT_New_Face(library, filename.c_str(), 0, &face))
	{
		std::cerr <<"Could not open the Font\n";
	}
	mFace =face;
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	
	FT_Set_Pixel_Sizes(face, 0, size);
	
	//Preload every printable char
	for (int i =0;i <256;i++)
	{
		FT_GlyphSlot glyph;
		
		if (!std::isprint(i)) continue;
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cerr <<"Could not load the character\n";
		}
		glyph =face->glyph;
		mGlyphs[i].width =glyph->bitmap.width;
		mGlyphs[i].height =glyph->bitmap.rows;
		mGlyphs[i].left =glyph->bitmap_left;
		mGlyphs[i].top =glyph->bitmap_top;
		mGlyphs[i].advance.x =glyph->advance.x;
		mGlyphs[i].advance.y =glyph->advance.y;
		mGlyphs[i].data.reserve(mGlyphs[i].width * mGlyphs[i].height);
		for (int y =mGlyphs[i].height - 1; y >=0;--y)
		{
			for (int x =mGlyphs[i].width - 1;x >=0;--x)
			{
				mGlyphs[i].data[x + y * mGlyphs[i].width] =glyph->bitmap.buffer[x + (mGlyphs[i].height - 1 - y) * mGlyphs[i].width];
			}
		}
		
		mAtlas.InsertImage(std::wstring(1, static_cast<wchar_t>(i)), mGlyphs[i].width, mGlyphs[i].height, mGlyphs[i].data.data());
	}
	
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mAtlas.GetAtlasSize(), mAtlas.GetAtlasSize(), 0, GL_RED, GL_UNSIGNED_BYTE, mAtlas.GetImageData().data());
	glBindTexture(GL_TEXTURE_2D, 0);
	
	for (auto ch: mGlyphs)
	{
		ch.second.data.clear();
	}
}

void Text::DrawText(std::wstring text, glm::vec2 position, glm::vec3 textColor)
{
	glm::vec3 pos;
	
	pos.x =position.x;
	pos.y =position.y + mSize;
	pos.z =0.f;
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(mVAO);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	mShader.UseShader();
	glUniformMatrix4fv(mShader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(mProjection));
	glUniform1f(mShader.GetUniformLocation("tex"), 0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	
	for (auto ch: text)
	{
		if (mAtlas.GetImage(std::wstring(1, ch)) ==glm::vec4(-1.f))
			addCharToAtlas(ch);
		
		Glyph* glyph =&mGlyphs[ch];
		float x, y;
		float top, left, width, height;
		
		if (ch =='\n')
		{
			pos.x =position.x;
			pos.y +=mSize;
			continue;
		}
		if (ch =='\r')
		{
			pos.y +=mSize;
			continue;
		}
		
		glUniform3fv(mShader.GetUniformLocation("textColor"), 1, glm::value_ptr(textColor));
		
		x =pos.x + glyph->left;
		y =pos.y - glyph->top;
		left =mAtlas.GetImage(std::wstring(1, ch)).x / mAtlas.GetAtlasSize();
		top =mAtlas.GetImage(std::wstring(1, ch)).y / mAtlas.GetAtlasSize();
		width =mAtlas.GetImage(std::wstring(1, ch)).z / mAtlas.GetAtlasSize();
		height =mAtlas.GetImage(std::wstring(1, ch)).w / mAtlas.GetAtlasSize();
		mVertices.clear();
		mVertices.push_back({{x					, y	+ glyph->height	, 0.f}, {}, {left		 , top}});
		mVertices.push_back({{x + glyph->width	, y	+ glyph->height	, 0.f}, {}, {left + width, top}});
		mVertices.push_back({{x					, y					, 0.f}, {}, {left		 , top + height}});
		mVertices.push_back({{x + glyph->width	, y 				, 0.f}, {}, {left + width, top + height}});
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_DYNAMIC_DRAW);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		pos.x +=(glyph->advance.x >>6);
		pos.y +=(glyph->advance.y >>6);
	}
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void Text::addCharToAtlas(wchar_t ch)
{
	FT_UInt glyph_index =FT_Get_Char_Index(mFace, ch);
	if (glyph_index ==0) {
		std::cerr <<"Glyph not found: " <<ch <<"\n";
		mAtlas.InsertImage(std::wstring(1, ch), 1, 1, (void*)"\0");
		return;
	}
	FT_GlyphSlot glyph;
		
	//if (!std::isprint(ch)) return;
	if (FT_Load_Char(mFace, ch, FT_LOAD_RENDER))
	{
		std::cerr <<"Could not load the character\n";
	}
	glyph =mFace->glyph;
	mGlyphs[ch].width =glyph->bitmap.width;
	mGlyphs[ch].height =glyph->bitmap.rows;
	mGlyphs[ch].left =glyph->bitmap_left;
	mGlyphs[ch].top =glyph->bitmap_top;
	mGlyphs[ch].advance.x =glyph->advance.x;
	mGlyphs[ch].advance.y =glyph->advance.y;
	mGlyphs[ch].data.reserve(mGlyphs[ch].width * mGlyphs[ch].height);
	for (int y =mGlyphs[ch].height - 1; y >=0;--y)
	{
		for (int x =mGlyphs[ch].width - 1;x >=0;--x)
		{
			mGlyphs[ch].data[x + y * mGlyphs[ch].width] =glyph->bitmap.buffer[x + (mGlyphs[ch].height - 1 - y) * mGlyphs[ch].width];
		}
	}
	
	mAtlas.InsertImage(std::wstring(1, static_cast<wchar_t>(ch)), mGlyphs[ch].width, mGlyphs[ch].height, mGlyphs[ch].data.data());
	
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mAtlas.GetAtlasSize(), mAtlas.GetAtlasSize(), 0, GL_RED, GL_UNSIGNED_BYTE, mAtlas.GetImageData().data());
	glBindTexture(GL_TEXTURE_2D, 0);
	
	mGlyphs[ch].data.clear();
}

void Text::GetTextSize(const std::wstring text, glm::vec2& size)
{
	float width_ =0.f;
	float height_ =mSize;
	
	for (auto& t: text)
	{
		if (t =='\n')
		{
			width_ =0;
			height_ +=mSize;
			continue;
		}
		if (t =='\r')
		{
			height_ +=mSize;
			continue;
		}
		
		width_ +=mGlyphs[t].advance.x >>6;
		height_ +=mGlyphs[t].advance.y >>6;
	}
	
	size.x =width_;
	size.y =height_;
}

glm::vec2 Text::GetTextSize(const std::wstring text)
{
	glm::vec2 size;
	
	GetTextSize(text, size);
	
	return size;
}

};