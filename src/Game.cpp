#include "Game.hpp"

#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include "gl_core_3_3.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/AutoPtr.h>

#include "States/TitleScreen.hpp"
#include "States/MenuScreen.hpp"
#include "States/GameScreen.hpp"

namespace fumasu
{

Game::Game() : mWindow({WindowWidth, WindowHeight, 24}, GameName)
{
	mWindow.setFramerateLimit(60);
	mWindow.setActive();
	ogl_LoadFunctions();
	ilInit();
	ilutRenderer(ILUT_OPENGL);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
	/// Initialize the Context
	mContext.SetGameSize(WindowWidth, WindowHeight);
	Poco::AutoPtr<Poco::SimpleFileChannel> pFileChannel(new Poco::SimpleFileChannel("output.log"));
	Poco::AutoPtr<Poco::PatternFormatter> pPatternFormatter(new Poco::PatternFormatter);
	Poco::AutoPtr<Poco::FormattingChannel> pFormattingChannel(new Poco::FormattingChannel(pPatternFormatter, pFileChannel));
	pPatternFormatter->setProperty("pattern", "%d.%m.%Y %H:%M:%S: %t");
	Poco::Logger::root().setChannel(pFormattingChannel);
	
	// Load all the used Fonts
	Text::Ptr sibila_26{new Text(&mContext)};
	sibila_26->LoadFromFile("res/Fonts/sibila.otf", 26);
	mContext.AddText("sibila_26", std::move(sibila_26));
	
	mStateManager.RegisterState<TitleScreen>(StateID::Title, &mContext);
	mStateManager.RegisterState<MenuScreen>(StateID::Menu, &mContext);
	mStateManager.RegisterState<GameScreen>(StateID::Game, &mContext);
	//mStateManager.RegisterState<OptionScreen>(StateID::Options, &mContext);
	
	mStateManager.Push(StateID::Game);
	
	glClearColor(0.3f, 0.3f, 0.7f, 1.0f);
}

Game::~Game()
{
}

void Game::Run()
{
	sf::Time timeSinceLastUpdate =sf::Time::Zero;
	sf::Clock clock;
	
	mContext.GetLogger("Default").information(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	//glGetString(GL_VERSION)
	
	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			HandleInput(event);
		}
		
		timeSinceLastUpdate +=clock.restart();
		while (timeSinceLastUpdate >=TimePerFrame)
		{
			timeSinceLastUpdate -=TimePerFrame;
			Update(TimePerFrame);
			
			if (mStateManager.isEmpty())
				mWindow.close();
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		Draw();
		
		mWindow.display();
	}
}

void Game::Update(sf::Time dt)
{
	mStateManager.Update(dt);
}

void Game::Draw()
{
	mStateManager.Draw();
}

void Game::HandleInput(sf::Event event)
{
	if (event.type ==sf::Event::Closed)
		mWindow.close();
		
	mStateManager.HandleEvent(event);
}


}