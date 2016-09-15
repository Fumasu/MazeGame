#include "Entity.hpp"

namespace fumasu
{
    
GLuint CBox::mVAO{0};
GLuint CBox::mVBO{0};

void Entity::addGroup(Group group) noexcept
{
    mGroupBitset[group] =true;
    mManager.addToGroup(this, group);
}

}