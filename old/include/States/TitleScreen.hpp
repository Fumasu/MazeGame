#ifndef TITLESCREEN_HPP
#define TITLESCREEN_HPP

#include <vector>

#include "State.hpp"
#include <SFML/System/Time.hpp>
#include "Resources/Shader.hpp"
#include "data_types.hpp"

#include "Manager/TextureManager.hpp"

namespace fumasu
{

class TitleScreen : public State
{
public:
	TitleScreen(StateManager* manager, Context* ctx);
	~TitleScreen();
	
	bool HandleEvent(sf::Event event);
	bool Update(sf::Time dt);
	void Draw();
	
private:
	const sf::Time DisplayTime =sf::seconds(5.f);
	sf::Time mElapsedTime =sf::Time::Zero;
	Shader mShader;
	GLuint mVBO, mVAO, mEBO;
	std::vector<Vertex> mVertices;
	std::vector<GLint> mIndices;
	TextureManager mTextures;
};

}

#endif // TITLESCREEN_HPP
