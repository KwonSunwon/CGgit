#ifndef __BG_H__
#define __BG_H__

#include "object.h"

class BG : public Object
{
private:
    GLint object;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> uvs;

    GLuint textureID;

public:
    BG();
    void render(GLuint shaderProgramID) override;
    void initTexture() override;
    void initBuffer() override;
};

#endif // __BG_H__