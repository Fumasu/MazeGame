#include "Quad.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Camera.hpp"

unsigned int Quad::referenceCounter;
std::vector<glm::vec3> Quad::mVertices;
GLuint Quad::mVBO =0;
GLuint Quad::mVAO =0;
Shader Quad::mShader;

Quad::Quad(const glm::vec3 position) : mPosition(position)
{
    referenceCounter++;
    if (!mVAO)
    {
        mVertices.push_back(glm::vec3(-1.f, -1.f,  0.f)); mVertices.push_back({0.f, 0.f, 0.f});
        mVertices.push_back(glm::vec3(-1.f,  1.f,  0.f)); mVertices.push_back({0.f, 1.f, 0.f});
        mVertices.push_back(glm::vec3( 1.f,  1.f,  0.f)); mVertices.push_back({1.f, 1.f, 0.f});
    
        mVertices.push_back(glm::vec3(-1.f, -1.f,  0.f)); mVertices.push_back({0.f, 0.f, 0.f});
        mVertices.push_back(glm::vec3( 1.f,  1.f,  0.f)); mVertices.push_back({1.f, 1.f, 0.f});
        mVertices.push_back(glm::vec3( 1.f, -1.f,  0.f)); mVertices.push_back({1.f, 0.f, 0.f});
    
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);
    
        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW);
    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(1 * sizeof(glm::vec3)));
        glEnableVertexAttribArray(1);
    
        mShader.loadFromFile("res/Shaders/quad.vs", "res/Shaders/quad.fs");
    
        glBindVertexArray(0);
    }
}

Quad::~Quad()
{
    referenceCounter--;
    if (mVAO && referenceCounter ==0)
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
        mVBO =0;
        mVAO =0;
        glUseProgram(0);
    }
}

void Quad::Draw(const sf::Window& window, Context& ctx)
{
    mShader.bind();
    
    glm::mat4 model;
    GLuint modelLoc, viewLoc, projLoc;
    
    const Texture* texture =ctx.GetTexture("res/Textures/quad.png");
    texture->bind();
    
    modelLoc =glGetUniformLocation(mShader.getNativeHandle(), "model");
    viewLoc =glGetUniformLocation(mShader.getNativeHandle(), "view");
    projLoc =glGetUniformLocation(mShader.getNativeHandle(), "projection");
    
    model =glm::translate(model, mPosition);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(ctx.GetCamera()->GetViewMatrix()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ctx.GetProjection()));
    
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
    
    glUseProgram(0);
}