#pragma once

#include <string>
#include <vector>
#include <SFML/Window.hpp>

#include "Context.hpp"
#include "Level.hpp"

#include "AABB.hpp"

class Game
{
public:
    Game(const unsigned int width, const unsigned int height, const std::string title);
    ~Game();
    
    bool Init();
    int Run();
    
private:
    void HandleEvents();
    void HandleInputs();
    
    bool running{true};
    sf::Window mWindow;
    unsigned int mWidth, mHeight;
    std::string mTitle;
    
    Context mContext;
    
    int lastMousePos[2] {-9999, -9999};
    
    std::vector<Level*> mObjects;
    
    AABB mPlayerBox;
};

