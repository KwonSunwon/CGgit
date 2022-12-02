#ifndef __CUBE_H__
#define __CUBE_H__

#include "object.h"

class Cube : public Object
{
private:
    GLint object;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> uvs;

    GLuint textureID[6];

public:
    Cube();
    void render(GLuint shaderProgramID) override;
    void initTexture() override;
    void initBuffer() override;

    void rotation(glm::vec3 rotate);
};

#endif // __CUBE_H__
