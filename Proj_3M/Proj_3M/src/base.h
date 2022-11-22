#pragma once
#include "object.h"

typedef class Base : public Object
{
public:
    void init(float color);
    void render(GLuint shaderProgramID) override;
    void update() override{};
} Base;