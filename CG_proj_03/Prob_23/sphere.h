#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "object.h"

class Sphere : public Object
{
private:
    static GLint object;
    static vector<glm::vec3> vertices;
    static vector<glm::vec3> normals;

public:
    Sphere();
    void render(GLuint shaderProgramID) override;
    void colorInit(glm::vec3 color);
    void initBuffer() override;

};

#endif // __SPHERE_H__
