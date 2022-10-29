#include "stdafx.h"

typedef class Camera
{
private:
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 ortho;

    glm::vec3 eye;
    glm::vec3 center;
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

public:
    Camera();

    void setEye(glm::vec3 eye);
    void setCenter(glm::vec3 center);
    void setUp(glm::vec3 up);

    void setFovy(float fovy);
    void setAspect(float aspect);
    void setzNear(float zNear);
    void setzFar(float zFar);

    void setLeft(float left);
    void setRight(float right);
    void setBottom(float bottom);
    void setTop(float top);

    // glm::mat4 getView();
    // glm::mat4 getProjection();

    void setCamera();
    glm::mat4 getView();
    glm::mat4 getProjection();
    glm::mat4 getOrtho();
} Camera;

void makeVertexShaders(char *file);
void makeFragmentShaders(char *file);
GLuint initShader(char *vertexFile, char *fragmentFile);

void initVAO(GLuint &VAO);
void initVBO_position(GLuint &VBO_position);
void initVBO_color(GLuint &VBO_color);
void initEBO(GLuint &EBO);

char *fileToBuf(char *fileName);