#include "firstCamera.h"
FirstCamera::FirstCamera() {}
FirstCamera::FirstCamera(int winWidth, int winHeight)
{
    eye = glm::vec3(0.0f, 0.0f, 0.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    pitch = 0.0f;
    yaw = 0.0f;
    angle = 0.0f;

    fovy = 45.0f;
    aspect = (float)winWidth / (float)winHeight;

    zNear = 0.1f;
    zFar = 100.0f;
}

void FirstCamera::init(glm::vec3 playerPos)
{
    eye = playerPos;
    eye.y = 2.0f;
}

void FirstCamera::setCamera(GLuint shaderID)
{
    target.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    target.y = sin(glm::radians(pitch));
    target.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    target = glm::normalize(target);

    view = glm::lookAt(eye, eye + target, up);
    projection = glm::perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void FirstCamera::update(glm::vec3 playerPos, float playerDir)
{
    eye = playerPos;
    eye.y = 2.0f;
    yaw = playerDir;
}