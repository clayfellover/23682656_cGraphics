#pragma once

#include <vector>
#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Texture
{
    unsigned int id;
    std::string type;
};

class Model
{
public:
    // attributes
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<Texture>   textures;
    unsigned int textureID;
    float ka, kd, ks, Ns;

    Model(const char* path);

    // draw model
    void draw(unsigned int shaderID);

    // textures
    void addTexture(const char* path, const std::string type);

    // clean it
    void deleteBuffers();

private:
    // buffers
    unsigned int VAO;
    unsigned int vertexBuffer;
    unsigned int uvBuffer;
    unsigned int normalBuffer;

    // load .obj file method
    bool loadObj(const char* path,
        std::vector<glm::vec3>& inVertices,
        std::vector<glm::vec2>& inUVs,
        std::vector<glm::vec3>& inNormals);

    // setup buffers
    void setupBuffers();

    // load texture
    unsigned int loadTexture(const char* path);

    // tangent space
    unsigned int tangentBuffer;  
    unsigned int bitangentBuffer; 

    // calculate tangent and bitangent vectors
    void calculateTangents();

};

