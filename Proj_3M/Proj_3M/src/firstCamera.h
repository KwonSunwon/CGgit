#pragma once
#include "camera.h"

class FirstCamera : public Camera
{
public:
    FirstCamera();
    FirstCamera(int winWidth, int winHeight);
    void update(glm::vec3 playerPos, float playerDir);
    void init(glm::vec3 playerPos);
    void setCamera(GLuint shaderID);
};