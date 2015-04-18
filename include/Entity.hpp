#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cassert>
#include <memory>
#include <bitset>
#include <vector>
#include <algorithm>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "gl_core_3_3.h"
#include "Context.hpp"
#include "data_types.hpp"
#include "Resources/Shader.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

constexpr double pi() { return std::atan(1)*4; }

namespace fumasu
{

class Component;
class Entity;
class EntityManager;

using ComponentID =std::size_t;
using Group =std::size_t;

namespace Internal
{
    inline ComponentID getUniqueComponentID() noexcept
    {
        static ComponentID lastID{0u};
        return lastID++;
    }
    
    template<typename T> inline ComponentID getComponentTypeID() noexcept
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        
        static ComponentID typeID{Internal::getUniqueComponentID()};
        return typeID;
    }
}

constexpr std::size_t maxComponents{32};
using ComponentBitset =std::bitset<maxComponents>;
using ComponentArray =std::array<Component*, maxComponents>;

constexpr std::size_t maxGroups{32};
using GroupBitset =std::bitset<maxGroups>;

class Component
{
public:
    virtual void Init() {}
    virtual void Update(sf::Time dt) {}
    virtual void Draw() {}
    
    virtual ~Component() =default;
    
    Entity* entity;
};

class Entity
{
public:
    Entity(EntityManager& manager) : mManager(manager) {}
    
    void Update(sf::Time dt)
    {
        for (auto& c: mComponents)
            c->Update(dt);
    }
    
    void Draw()
    {
        for (auto& c: mComponents)
            c->Draw();
    }
    
    bool isAlive() const { return mAlive; }
    void Destroy() { mAlive =false; }
    
    template<typename T> bool hasComponent() const
    {
        return mComponentBitset[Internal::getComponentTypeID<T>()];
    }
    
    bool hasGroup(Group group) const noexcept
    {
        return mGroupBitset[group];
    }
    
    void addGroup(Group group) noexcept;
    void delGroup(Group group) noexcept
    {
        mGroupBitset[group] =false;
    }
    
    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        assert(!hasComponent<T>());
        
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity =this;
        std::unique_ptr<Component> uPtr{c};
        mComponents.emplace_back(std::move(uPtr));
        
        mComponentArray[Internal::getComponentTypeID<T>()] =c;
        mComponentBitset[Internal::getComponentTypeID<T>()] =true;
        
        c->Init();
        return *c;
    }
    
    template<typename T> T& getComponent() const
    {
        assert(hasComponent<T>());
        
        auto ptr(mComponentArray[Internal::getComponentTypeID<T>()]);
        return *reinterpret_cast<T*>(ptr);
    }

private:
    EntityManager& mManager;
    bool mAlive{true};
    std::vector<std::unique_ptr<Component>> mComponents;
    ComponentArray mComponentArray;
    ComponentBitset mComponentBitset;
    GroupBitset mGroupBitset;
};

class EntityManager
{
public:
    EntityManager() =default;
    
    void Update(sf::Time dt)
    {
        for (auto& e: mEntities)
            e->Update(dt);
    }
    
    void Draw()
    {
        for (auto& e: mEntities)
            e->Draw();
    }
    
    void addToGroup(Entity* entity, Group group)
    {
        mGroupedEntities[group].emplace_back(entity);
    }
    
    std::vector<Entity*>& getEntitiesByGroup(Group group)
    {
        return mGroupedEntities[group];
    }
    
    void Refresh()
    {
        for (auto i(0u);i <maxGroups;++i)
        {
            auto& v(mGroupedEntities[i]);
            
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                [i](Entity* entity)
                {
                    return !entity->isAlive() || !entity->hasGroup(i);
                }), 
                std::end(v)
            );
        }
        
        mEntities.erase(
            std::remove_if(std::begin(mEntities), std::end(mEntities),
            [](const std::unique_ptr<Entity>& entity)
            {
                return !entity->isAlive();
            }), 
            std::end(mEntities)
        );
    }
    
    Entity& addEntity()
    {
        Entity* e(new Entity(*this));
        std::unique_ptr<Entity> uPtr{e};
        mEntities.emplace_back(std::move(uPtr));
        
        return *e;
    }

private:
    std::vector<std::unique_ptr<Entity>> mEntities;
    std::array<std::vector<Entity*>, maxGroups> mGroupedEntities;
};

class CPosition : public Component
{
public:
    CPosition() =default;
    CPosition(const glm::vec3& position) : mPosition{position} {}
    
    float x() const noexcept { return mPosition.x; }
    float y() const noexcept { return mPosition.y; }
    float z() const noexcept { return mPosition.z; }
    glm::vec3 GetVect() const noexcept { return mPosition; }

private:
    glm::vec3 mPosition;
};

class CBox : public Component
{
public:
    CBox(Context* ctx, float size) : mSize{size}, mContext{ctx}
    {
        float sh =(mSize / 2.f);
        mVertices.push_back({{-sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, +sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, +sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, +sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        
        mVertices.push_back({{+sh, -sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, -sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, -sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        
        mVertices.push_back({{-sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, -sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, +sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{-sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        
        mVertices.push_back({{+sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, -sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, +sh, -sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, +sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
        mVertices.push_back({{+sh, -sh, +sh}, {0.f, 0.f, 0.f}, {0.f, 0.f}});
    }
    
    ~CBox()
    {
        if (mVAO !=0)
            glDeleteVertexArrays(1, &mVAO);
        if (mVBO !=0)
            glDeleteBuffers(1, &mVBO);
    }
    
    void Init() override
    {
        cPosition =&entity->getComponent<CPosition>();
        
        if (mVAO ==0)
            glGenVertexArrays(1, &mVAO);
        if (mVBO ==0)
            glGenBuffers(1, &mVBO);
        
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        
        glBindVertexArray(0);
        
        mShader.LoadFromFile(Shader::FragmentShader, "res/Shader/test_frag.glsl");
        mShader.LoadFromFile(Shader::VertexShader, "res/Shader/test_vert.glsl");
        mShader.UseShader();
        
        std::cerr <<mShader.GetErrorLog();
        
        size_t width, height;
        mContext->GetGameSize(width, height);
        mProjection =glm::perspective(45.f, 4.f/3.f, 1.f, 1000.f);
    }
    
    void Draw() override
    {
        static float s =0;
        s +=.01f,
        glBindVertexArray(mVAO);
        mShader.UseShader();
        
        glUniformMatrix4fv(mShader.GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(mProjection));
        mModel =glm::translate(glm::mat4(), cPosition->GetVect());
        mModel =glm::rotate(mModel, s, {0.f, 1.f, 0.f});
        glUniformMatrix4fv(mShader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(mModel));
        
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        
        glBindVertexArray(0);
        glUseProgram(0);
    }
    
private:
    CPosition* cPosition{nullptr};
    float mSize;
    static GLuint mVAO, mVBO;
    std::vector<Vertex> mVertices;
    Shader mShader;
    Context* mContext{nullptr};
    glm::mat4 mProjection;
    glm::mat4 mModel;
};

}
#endif // ENTITY_HPP
