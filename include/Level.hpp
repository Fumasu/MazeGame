#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "gl_core_3_3.h"
#include "Context.hpp"
#include "Shader.hpp"

namespace sf
{
	class Window;
}

class Level
{
public:
	Level (Context& ctx);
	~Level ();
	
	bool loadFromFile (const std::string filename);
	
	void Draw (const sf::Window& window, Context& ctx);
	
private:
	std::vector<glm::vec3> mVertices;
	GLuint mVBO, mVAO, mEBO;
	Shader mShader;
	std::string mTitle;
	glm::vec3 mPosition;
	std::vector<unsigned int> mIndices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mUV;
	std::vector<std::string> mTextures;
	Context& mCtx;
	
	std::map<unsigned int, unsigned long> mTest;
};