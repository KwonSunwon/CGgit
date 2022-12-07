#ifndef __PYRAMID_H__
#define __PYRAMID_H__

#include "object.h"

class Pyramid : public Object
{
private:
    GLint object;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> uvs;

    GLuint textureID[5];

public:
    Pyramid();
    void render(GLuint shaderProgramID) override;
    void initTexture() override;
    void initBuffer() override;
    void update() override;
};

#endif