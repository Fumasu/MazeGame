#include "States/State.hpp"

#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include "Manager/StateManager.hpp"

namespace fumasu
{

State::State(StateManager* manager, Context* ctx) : mStateManager(manager)
{
}

}
