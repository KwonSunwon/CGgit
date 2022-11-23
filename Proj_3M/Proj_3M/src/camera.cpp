#include "camera.h"

// Camera defualt constructor
Camera::Camera()
{
    pitch = 0.f;
    yaw = -90.f;
    angle = 0.f;

    eye = glm::vec3(0.0f, 0.f, 3.f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    fovy = 45.0f;
    aspect = 800.0f / 800.0f;

    zNear = 0.1f;
    zFar = 100.0f;

    left = -1.0f;
    right = 1.0f;
    bottom = -1.0f;
    top = 1.0f;

    radius = -120.f;
}

void Camera::setCamera(GLuint shaderProgramID, int type) // 0 = perspective, 1 = ortho
{
    // target.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    // target.y = sin(glm::radians(pitch));
    // target.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    // target = glm::normalize(target);

    view = glm::lookAt(eye, target, up);
    projection = glm::perspective(fovy, aspect, zNear, zFar);
    ortho = glm::ortho(left, right, bottom, top, zNear, zFar);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    if (type == 0)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    }
    else if (type == 1)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(ortho));
    }
}

void Camera::setEye(glm::vec3 eye) { this->eye = eye; }
void Camera::setTarget(glm::vec3 target) { this->target = target; }
void Camera::setUp(glm::vec3 up) { this->up = up; }

glm::vec3 Camera::getEye() { return eye; }
glm::vec3 Camera::getTarget() { return target; }
glm::vec3 Camera::getUp() { return up; }

void Camera::setPitch(float pitch) { this->pitch = pitch; }
void Camera::setYaw(float yaw) { this->yaw = yaw; }
void Camera::setAngle(float angle) { this->angle = angle; }

float Camera::getPitch() { return pitch; }
float Camera::getYaw() { return yaw; }
float Camera::getAngle() { return angle; }

void Camera::setFovy(float fovy) { this->fovy = fovy; }
void Camera::setAspect(float aspect) { this->aspect = aspect; }
void Camera::setzNear(float zNear) { this->zNear = zNear; }
void Camera::setzFar(float zFar) { this->zFar = zFar; }

void Camera::setLeft(float left) { this->left = left; }
void Camera::setRight(float right) { this->right = right; }
void Camera::setBottom(float bottom) { this->bottom = bottom; }
void Camera::setTop(float top) { this->top = top; }

glm::mat4 Camera::getView() { return view; }
glm::mat4 Camera::getProjection() { return projection; }
glm::mat4 Camera::getOrtho() { return ortho; }

void Camera::update()
{
    if (isRotating)
    {
        rotate();
    }
}

void Camera::setRotating(int isRotating)
{
    this->isRotating = isRotating;

    if (isRotating)
    {
        radius = -120.f;
    }
}
int Camera::getRotating() { return isRotating; }

void Camera::rotate()
{
    if (isRotating == 1)
    {
        angle += 0.01f;
        eye.x = radius * cos(angle);
        eye.z = radius * sin(angle);
    }
    else if (isRotating == -1)
    {
        angle -= 0.01f;
        eye.x = radius * cos(angle);
        eye.z = radius * sin(angle);
    }
}