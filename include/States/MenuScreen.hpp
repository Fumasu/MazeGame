#ifndef MENUSCREEN_HPP
#define MENUSCREEN_HPP

#include <vector>

#include "States/State.hpp"
#include "Manager/TextureManager.hpp"
#include "Resources/Shader.hpp"
#include "data_types.hpp"
#include "Resources/Text.hpp"
#include "Context.hpp"

namespace fumasu
{
	
class MenuScreen : public State
{
public:
	MenuScreen(StateManager* manager, Context* ctx);
	~MenuScreen();
	
	bool Update(sf::Time dt);
	bool HandleEvent(sf::Event event);
	void Draw();
	
private:
	Context* mContext;
	TextureManager mTextures;
	Shader mShader;
	GLuint mVBO, mVAO, mEBO;
	std::vector<Vertex> mVertices;
	std::vector<GLint> mIndices;
	glm::mat4 mProjection, mModel;
};

}

#endif // MENUSCREEN_HPP
