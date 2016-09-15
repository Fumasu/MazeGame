#include "States/GameScreen.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

namespace fumasu {

GameScreen::GameScreen(StateManager* manger, Context* ctx) : State(manger, ctx), mContext{ctx}
{
    auto& entity(mEntityManager.addEntity());
    
    entity.addComponent<CPosition>(glm::vec3(0.f, 0.f, -10.f));
    entity.addComponent<CBox>(mContext, 1.f);
}

GameScreen::~GameScreen()
{
}

void GameScreen::Draw()
{
    mEntityManager.Draw();
}

bool GameScreen::HandleEvent(sf::Event event)
{
	return true;
}

bool GameScreen::Update(sf::Time dt)
{
    mEntityManager.Update(dt);
    
	return true;
}

};