#include "Game.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "config.h"
#include "gl_core_3_3.h"
#include "Level.hpp"
#include "Camera.hpp"

/**
 * @brief 
 * @param width The Width of the Game Window
 * @param height The Height of the Game Window
 * @param title The Title of the Game Window
 */
Game::Game(const unsigned int width, const unsigned int height, const std::string title) : mWidth(width), mHeight(height), mTitle(title)
{
}

Game::~Game()
{
}

/**
 * @brief Initializes the Window, OpenGL and some more
 * @return Returns true, if the Window was created and initialized (OpenGL, Textures, ...)
 */
bool Game::Init()
{
    sf::ContextSettings settings;
    
    settings.majorVersion =3;
    settings.minorVersion =3;
    settings.attributeFlags =sf::ContextSettings::Core;
    settings.depthBits =24;
    settings.stencilBits =8;
    
    mWindow.create({mWidth, mHeight}, mTitle + " " + std::to_string(MazeGame_VERSION_MAJOR) + "." + std::to_string(MazeGame_VERSION_MINOR), sf::Style::Titlebar | sf::Style::Close, settings);
    
    ogl_LoadFunctions();
    
    if (ogl_LOAD_FAILED)
        return false;
    
    if (ogl_LOAD_SUCCEEDED)
    {
        if (!ogl_IsVersionGEQ(3, 3))
        {
            std::cerr <<"The OpenGL Profile Requirements are not met!\n";
            return false;
        }
        std::cout <<"OpenGL Version: " <<ogl_GetMajorVersion() <<"." <<ogl_GetMinorVersion() <<"\n";
        std::cout <<"OpenGL Vendor: " <<glGetString(GL_VENDOR) <<"\n";
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    glViewport(0, 0, mWidth, mHeight);
    
    mContext.SetProjection(glm::perspective(45.f, mWidth / static_cast<float>(mHeight), 0.1f, 1000.f));
    
    mContext.SetCamera(Camera({0.f, 0.f, 10.f}));
    mContext.GetCamera()->MovementSpeed =0.05f;
    
//  mObjects.emplace_back(new Quad({0.f, 0.f, 0.f}));
//    mObjects.emplace_back(new Quad({0.f, 3.f, 0.f}));
	mObjects.emplace_back (new Level(mContext));
	mObjects[0]->loadFromFile ("res/Maps/quad.flf");
    
    return true;
}

/**
 * @brief The Main Game Loop
 * @return Always returns EXIT_SUCCESS
 */
int Game::Run()
{
    sf::Clock clock;
    sf::Time deltaTime;
    sf::Time TimePerFrame =sf::seconds(1 / 60.f);
    
    while (running)
    {
        deltaTime +=clock.getElapsedTime();
        
        while (deltaTime >=TimePerFrame)
        {
            HandleEvents();
            HandleInputs();
            deltaTime -=TimePerFrame;
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (auto& obj: mObjects)
            obj->Draw(mWindow, mContext);
        
        mWindow.display();
    }
    
    for (auto obj: mObjects)
    {
        delete obj;
        obj =nullptr;
    }
    
    mWindow.close();
    
    return EXIT_SUCCESS;
}

void Game::HandleEvents()
{
    sf::Event event;
    
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                running =false;
                break;
            }
            case sf::Event::Resized:
            {
                glViewport(0, 0, event.size.height, event.size.height);
                break;
            }
            case sf::Event::MouseMoved:
            {
                break;
            }
            case sf::Event::KeyPressed:
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                    {
                        running =false;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default: break;
        }
    }
}

void Game::HandleInputs()
{
    static bool firstRun =true;
    sf::Time time =sf::seconds(1 / 60.f);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        mContext.GetCamera()->ProcessKeyboard(FORWARD, time.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        mContext.GetCamera()->ProcessKeyboard(BACKWARD, time.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        mContext.GetCamera()->ProcessKeyboard(LEFT, time.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        mContext.GetCamera()->ProcessKeyboard(RIGHT, time.asSeconds());
    
    if (firstRun)
    {
        sf::Mouse::setPosition({static_cast<int>(mWidth) / 2, static_cast<int>(mHeight) / 2}, mWindow);
        firstRun =false;
    }
    sf::Vector2i mousePos =sf::Mouse::getPosition(mWindow);
    GLfloat xoffset =mousePos.x - (mWidth / 2.f);
//    GLfloat yoffset =(mHeight / 2.f) - mousePos.y;
    GLfloat yoffset =mousePos.y - (mHeight / 2.f);  //Inverted Mouse
    sf::Mouse::setPosition({static_cast<int>(mWidth) / 2, static_cast<int>(mHeight) / 2}, mWindow);
    mContext.GetCamera()->ProcessMouseMovement(xoffset, yoffset);
}
