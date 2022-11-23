#pragma once
#include "object.h"

class Player : public Object
{
private:
    float speed = 0.3;
    int dir_x = 0;
    int dir_z = 0;

    float direction = 0.f;

    glm::vec3 prevPos;

    int viewMode = THIRD;

    void move();

public:
    void init() override;
    void render(GLuint shaderProgramID) override;
    void update() override;

    void keyInput(unsigned int key, int type); // 0: key down, 1: key up

    void handleCollision();

    void setViewMode(int mode);
    void setDirection(float dir);
    float getDirection();
};