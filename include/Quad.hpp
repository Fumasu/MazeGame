#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "gl_core_3_3.h"
#include "Context.hpp"
#include "Shader.hpp"

namespace sf
{
    class Window;
}

class Quad
{
public:
    Quad(const glm::vec3 position);
    ~Quad();
    
    void Draw(const sf::Window& window, Context& ctx);

private:
    static unsigned int referenceCounter;
    static std::vector<glm::vec3> mVertices;
    static GLuint mVBO, mVAO;
    static Shader mShader;
    
    glm::vec3 mPosition;
};

