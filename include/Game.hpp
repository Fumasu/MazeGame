#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <SFML/Window/Window.hpp>
#include "Manager/StateManager.hpp"
#include "Manager/TextureManager.hpp"

namespace sf
{
	class Event;
};

namespace fumasu
{

class Game
{
public:
	Game();
	~Game();
	
	void Run();

private:
	const std::string GameName{"MazeGame"};
	const unsigned int WindowWidth{1024}, WindowHeight{768};
	const sf::Time TimePerFrame =sf::seconds(1.f / 60.f);
	
	sf::Window mWindow;
	fumasu::StateManager mStateManager;
	
	void Update(sf::Time dt);
	void Draw();
	void HandleInput(sf::Event event);
	TextureManager mTextureManager;
	Context mContext;
};

}

#endif // GAME_HPP
