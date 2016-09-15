#include "States/MenuScreen.hpp"

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include "Manager/StateManager.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fumasu
{

MenuScreen::MenuScreen(StateManager* manager, Context* ctx) : State(manager, ctx), mContext(ctx)
{
	mShader.LoadFromFile(Shader::VertexShader, "res/Shader/MenuScreen/vertex.glsl");
	mShader.LoadFromFile(Shader::FragmentShader, "res/Shader/MenuScreen/fragment.glsl");
	
	mVertices.push_back({{0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 0.f}});
	mVertices.push_back({{0.f, 110.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f}});
	mVertices.push_back({{600.f, 110.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f}});
	mVertices.push_back({{600.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f}});
	
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
	
	size_t width, height;
	mContext->GetGameSize(width, height);
	mProjection =glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f);
	mModel =glm::mat4(1.f);
	mModel =glm::translate(mModel, {212.f, 100.f, 0.f});
}

MenuScreen::~MenuScreen()
{
}

bool MenuScreen::Update(sf::Time dt)
{
	return true;
}

bool MenuScreen::HandleEvent(sf::Event event)
{
	if (event.type ==sf::Event::KeyPressed && event.key.code ==sf::Keyboard::Escape)
	{
		mStateManager->Clear();
	}
	
	if (event.type ==sf::Event::KeyReleased && event.key.code ==sf::Keyboard::S)
	{
		mStateManager->Clear();
		mStateManager->Push(StateID::Game);
	}
	
	return true;
}

void MenuScreen::Draw()
{
	glm::vec2 pos;
	
	mContext->GetGameSize(pos);
	pos.x /=2.f;
	pos.y /=2.f;
	pos.x -=mContext->GetText("sibila_26")->GetTextSize(L"Fumasu mitß äöüß !!").x / 2.f;
	pos.y -=mContext->GetText("sibila_26")->GetTextSize(L"Fumasu mitß äöüß !!").y / 2.f;
	mContext->GetText("sibila_26")->DrawText(L"Fumasu mitß äöüß !!", pos, {1.f, 0.f, 1.f});
}

}

