#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include "States/State.hpp"
#include "Entity.hpp"

namespace fumasu {

class GameScreen : public State
{
public:
	GameScreen(StateManager* manager, Context* ctx);
	virtual ~GameScreen();
	
	bool Update(sf::Time dt) override;
	bool HandleEvent(sf::Event event) override;
	void Draw() override;
	
private:
	Context* mContext;
    EntityManager mEntityManager;
};

}

#endif