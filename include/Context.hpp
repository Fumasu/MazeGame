#pragma once

#include <Thor/Resources.hpp>

#include "Camera.hpp"
#include "Texture.hpp"

class Context
{
public:
    Context() =default;
    ~Context() =default;
    
    Camera* GetCamera()
    {
        return &mCamera;
    }
    void SetCamera(const Camera cam)
    {
        mCamera =cam;
    }
    
    const glm::mat4 GetProjection() const
    {
        return mProjection;
    }
    void SetProjection(const glm::mat4 projection)
    {
        mProjection =projection;
    }
    
    const Texture* GetTexture(std::string name)
    {
        return &mResources.acquire(name, thor::Resources::fromFile<Texture>(name), thor::Resources::Reuse);
    }

private:
    Camera mCamera;
    glm::mat4 mProjection;
    thor::ResourceHolder<Texture, std::string> mResources;
};