#pragma once

#include <glm/glm.hpp>

#include "Drawable.hpp"

class Triangle : public Drawable
{
public:
    Triangle(Context& ctx);
    ~Triangle();
    
private:
    glm::vec3 mVertices[3];
};