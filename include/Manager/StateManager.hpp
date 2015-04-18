#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <map>
#include <vector>
#include "StateIdentifiers.hpp"
#include "States/State.hpp"
#include "Manager/TextureManager.hpp"

namespace sf
{
	class Time;
	class Event;
};

namespace fumasu
{

class StateManager
{
public:
	StateManager();
	~StateManager();

	void Push(StateID id);
	void Pop();
	void Clear();
	bool isEmpty() const;
	
	void HandleEvent(sf::Event event);
	void Update(sf::Time dt);
	void Draw();
	
	template<typename T>
	void RegisterState(StateID id, Context* ctx);
	
private:
	enum Action
	{
		ActionPush,
		ActionPop,
		ActionClear
	};
	struct PendingChange
	{
		PendingChange() =delete;
		PendingChange(Action action, StateID id =None) : action{action}, stateID{id}
		{ }
		
		Action action;
		StateID stateID;
	};
	std::vector<State::Ptr> mStates;
	std::map<StateID, std::function<State::Ptr()>> mFactories;
	std::vector<PendingChange> mPendingChanges;
	
	void ApplyPendingChanges();
};

template<typename T>
void StateManager::RegisterState(StateID id, Context* ctx)
{
	mFactories[id] =[this, ctx] ()
	{
		return State::Ptr(new T(this, ctx));
	};
}

}

#endif // STATEMANAGER_HPP
