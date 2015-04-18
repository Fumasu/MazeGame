#ifndef STATE_HPP
#define STATE_HPP

#include <memory>
#include "StateIdentifiers.hpp"
#include "Context.hpp"

namespace fumasu
{
	class StateManager;
};

namespace sf
{
	class Time;
	class Event;
};

namespace fumasu
{

class State
{
public:
	using Ptr =std::unique_ptr<State>;
	
	State(StateManager* manager, Context* ctx);
	virtual ~State() =default;
	
	virtual bool Update(sf::Time dt) =0;
	virtual bool HandleEvent(sf::Event event) =0;
	virtual void Draw() =0;
	
protected:
	StateManager* mStateManager =nullptr;
};

}

#endif // STATE_HPP
