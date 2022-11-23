#pragma once
#include "object.h"

typedef class Pillar : public Object
{
private:
    float height;
    float step;

    float speed;

    bool isAnimating;
    bool isFall;
    glm::vec2 width;

    void fall();
    void move();

public:
    void init(int row, int col, int idx);
    void render(GLuint shaderProgramID) override;
    void update() override;

    void setSpeed(float speed) { this->speed = speed; }
    void setHeight(float height) { this->height = height; }
    void setAnimating(bool isAnimating) { this->isAnimating = isAnimating; }

    float getSpeed() { return this->speed; }
    float getHeight() { return this->height; }
    float getAnimating() { return this->isAnimating; }

    void reset();
} Pillar;

typedef class PillarManager
{
private:
    vector<Pillar> pillars;
    int row, col;

    bool isLowHeight;

public:
    void init(int row, int col);
    void render(GLuint shaderProgramID);
    void update();

    void makeMaze();
    void setAnimating(bool isAnimating);

    void setLowestHeight();

    void speedUp();
    void speedDown();

    int getPillarCount() { return this->pillars.size(); }
    Pillar getPillar(int idx) { return this->pillars[idx]; }

    void reset();
} PillarManager;