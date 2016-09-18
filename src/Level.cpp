#include "Level.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Level::Level (Context& ctx) : mVBO(0), mVAO(0), mEBO(0), mCtx(ctx)
{
    mShader.loadFromFile ("res/Shaders/quad.vs", "res/Shaders/quad.fs");
}

Level::~Level ()
{
    if (mVAO)
    {
        glDeleteBuffers (1, &mVAO);
        mVAO =0;
    }
    if (mVBO)
    {
        glDeleteBuffers (1, &mVBO);
        mVBO =0;
    }
    if (mEBO)
    {
        glDeleteBuffers (1, &mEBO);
        mEBO =0;
    }
}

bool Level::loadFromFile (const std::string filename)
{
    if (mVAO)
    {
        glDeleteBuffers (1, &mVAO);
        mVAO =0;
    }
    if (mVBO)
    {
        glDeleteBuffers (1, &mVBO);
        mVBO =0;
    }
    if (mEBO)
    {
        glDeleteBuffers (1, &mEBO);
        mEBO =0;
    }

    std::vector<unsigned int> textureIDs;
    std::map<unsigned int, std::vector<unsigned int>> indices;
    std::map<unsigned int, std::vector<glm::vec2>> uv;
    std::map<unsigned int, std::vector<glm::vec3>> normals;
    unsigned int ID =0;
    //////////////////////////////////////////
    std::ifstream Datei;
    std::string line;

    Datei.open (filename);

    if (!Datei.is_open())
    {
        std::cerr <<"ERROR::LEVEL::File not found\n";

        return false;
    }

    std::getline (Datei, line);
    if (line.compare ("FLF") !=0)
    {
        std::cerr <<"ERROR::LEVEL::Not a Valid Level File\n";

        return false;
    }

    std::getline (Datei, line);
    mTitle =line;

    while (!Datei.eof())
    {
        std::getline (Datei, line);
        switch (line[0])
        {
            case 'T':
            {
                line.erase (0, 2);
                mTextures.push_back (line);
                textureIDs.push_back (ID++);
                break;
            }
            case 'V':
            {
                float x, y, z, ux, uy, nx, ny, nz;
                line.erase (0, 2);
                std::stringstream sstr;

                sstr <<line;
                sstr >>x >>y >>z >>ux >>uy >>nx >>ny >>nz;
                mVertices.push_back (glm::vec3(x, y, z));
                mUV.push_back (glm::vec2(ux, uy));
                mNormals.push_back (glm::vec3(nx, ny, nz));
                break;
            }
            case 'M':
            {
                unsigned int t, i1, i2, i3;

                line.erase (0, 2);
                std::stringstream sstr;

                sstr <<line;
                sstr >>t >>i1 >>i2 >>i3;

                indices[t].push_back (i1);
                indices[t].push_back (i2);
                indices[t].push_back (i3);

                mTest[t]++;

                break;
            }
        }
    }

    Datei.close();

    for (auto& t: textureIDs)
    {
        for (unsigned int r: indices[t])
        {
            mIndices.push_back (r);
        }
        for (auto& r: normals[t])
        {
            mNormals.push_back (r);
        }
        for (auto& r: uv[t])
        {
            mUV.push_back (r);
        }
    }
    //////////////////////////////////////////

    glGenVertexArrays (1, &mVAO);
    glBindVertexArray (mVAO);

    glGenBuffers (1, &mVBO);
    glBindBuffer (GL_ARRAY_BUFFER, mVBO);

    glGenBuffers (1, &mEBO);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

    glBufferData (GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3) + mUV.size() * sizeof(glm::vec2) + mNormals.size() * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);

    glBufferSubData (GL_ARRAY_BUFFER, 0, mVertices.size() * sizeof(glm::vec3), &mVertices[0]);
    glBufferSubData (GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), mUV.size() * sizeof(glm::vec2), &mUV[0]);
    glBufferSubData (GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3) + mUV.size() * sizeof(glm::vec2), mNormals.size() * sizeof(glm::vec3), &mNormals[0]);

    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glEnableVertexAttribArray (0);

    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(mVertices.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray (1);

    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)(mVertices.size() * sizeof(glm::vec3) + mUV.size() * sizeof(glm::vec2)));
    glEnableVertexAttribArray (2);

    for (auto& t: mTextures)
            mCtx.LoadTexture (t);

    glBindVertexArray (0);

    return true;
}

void Level::Draw (const sf::Window& window, Context& ctx)
{
    mShader.bind();

    glm::mat4 model;
    GLuint modelLoc, viewLoc, projLoc;

    modelLoc =glGetUniformLocation(mShader.getNativeHandle(), "model");
    viewLoc =glGetUniformLocation(mShader.getNativeHandle(), "view");
    projLoc =glGetUniformLocation(mShader.getNativeHandle(), "projection");
    model =glm::translate(model, mPosition);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(ctx.GetCamera()->GetViewMatrix()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ctx.GetProjection()));
	
    glBindVertexArray (mVAO);
    int id =0;
    int offset =0;
    for (auto& t: mTextures)
    {
        ctx.GetTexture(t)->bind();
        glDrawElements (GL_TRIANGLES, offset * 3 + mTest[id] * 3, GL_UNSIGNED_INT, 0);
        offset +=mTest[id++];
    }
    glBindVertexArray (0);

    glUseProgram (0);
}