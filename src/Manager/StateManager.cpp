#include "Manager/StateManager.hpp"

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace fumasu
{

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
}

void StateManager::Push(StateID id)
{
	//mStates.push_back(mFactories[id]());
	mPendingChanges.push_back(PendingChange(ActionPush, id));
}

void StateManager::Pop()
{
	//mStates.pop_back();
	mPendingChanges.push_back(PendingChange(ActionPop));
}

void StateManager::Clear()
{
	//mStates.clear();
	mPendingChanges.push_back(PendingChange(ActionClear));
}

void StateManager::HandleEvent(sf::Event event)
{
	for (auto itr =mStates.rbegin();itr !=mStates.rend();++itr)
	{
		if (!(*itr)->HandleEvent(event))
			break;
	}
	
	ApplyPendingChanges();
}

void StateManager::Update(sf::Time dt)
{
	for (auto itr =mStates.rbegin();itr !=mStates.rend();++itr)
	{
		if (!(*itr)->Update(dt))
			break;
	}
	
	ApplyPendingChanges();
}

void StateManager::Draw()
{
	for (auto& state: mStates)
		state->Draw();
}

bool StateManager::isEmpty() const
{
	return mStates.empty();
}

void StateManager::ApplyPendingChanges()
{
	for (auto& change: mPendingChanges)
	{
		switch (change.action)
		{
			case  ActionPush:
				mStates.push_back(mFactories[change.stateID]());
				break;
			case ActionPop:
				mStates.pop_back();
				break;
			case ActionClear:
				mStates.clear();
				break;
		}
	}
	
	mPendingChanges.clear();
}

}

