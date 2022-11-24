#pragma once
#include "stdafx.h"

typedef class Camera
{
protected:
    float pitch;
    float yaw;

    float angle;

    glm::mat4 viewTransform;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 ortho;

    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;

    // Perspective
    float fovy;
    float aspect;

    // Ortho
    float left;
    float right;
    float bottom;
    float top;

    float zNear;
    float zFar;

    int isRotating; // 0 = no, 1 = positive, -1 = negative
    float radius;

public:
    Camera();
    Camera(int winWidth, int winHeight);

    void setEye(glm::vec3 eye);
    void setTarget(glm::vec3 target);
    void setUp(glm::vec3 up);

    glm::vec3 getEye();
    glm::vec3 getTarget();
    glm::vec3 getUp();

    void setPitch(float pitch);
    void setYaw(float yaw);
    void setAngle(float angle);

    float getPitch();
    float getYaw();
    float getAngle();

    void setFovy(float fovy);
    void setAspect(float aspect);
    void setzNear(float zNear);
    void setzFar(float zFar);

    void setLeft(float left);
    void setRight(float right);
    void setBottom(float bottom);
    void setTop(float top);

    void setCamera(GLuint shaderProgramID, int type); // 0: Perspective, 1: Ortho
    glm::mat4 getView();
    glm::mat4 getProjection();
    glm::mat4 getOrtho();

    void update();

    void setRotating(int isRotating);
    int getRotating();

    void rotate();

} Camera;