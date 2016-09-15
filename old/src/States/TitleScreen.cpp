#include "States/TitleScreen.hpp"

#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include "Manager/StateManager.hpp"

namespace fumasu
{

TitleScreen::TitleScreen(StateManager* manager, Context* ctx) : State(manager, ctx)
{
	mShader.LoadFromFile(Shader::FragmentShader, "res/Shader/TitleScreen/fragment.glsl");
	mShader.LoadFromFile(Shader::VertexShader, "res/Shader/TitleScreen/vertex.glsl");
	mShader.UseShader();
	
	std::cerr <<mShader.GetErrorLog();
	
	mVertices.push_back({{-1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 0.f}});
	mVertices.push_back({{-1.f, +1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f}});
	mVertices.push_back({{+1.f, +1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f}});
	mVertices.push_back({{+1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f}});
	
	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);
	mIndices.push_back(0);
	mIndices.push_back(2);
	mIndices.push_back(3);
	
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	
	mTextures.Load("TitleScreen", "res/Textures/TitleScreen/title.jpg");
}

TitleScreen::~TitleScreen()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
	glDeleteVertexArrays(1, &mVAO);
}

bool TitleScreen::HandleEvent(sf::Event event)
{
	return true;
}

bool TitleScreen::Update(sf::Time dt)
{
	mElapsedTime +=dt;
	
	if (mElapsedTime >=DisplayTime)
	{
		mStateManager->Clear();
		mStateManager->Push(Menu);
	}
	
	return true;
}

void TitleScreen::Draw()
{
	mShader.UseShader();
	glBindVertexArray(mVAO);
	glBindTexture(GL_TEXTURE_2D, mTextures.Get("TitleScreen")->GetHandle());
	glUniform1f(mShader.GetUniformLocation("alpha"), mElapsedTime.asSeconds() / 5.f);
	
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

}

