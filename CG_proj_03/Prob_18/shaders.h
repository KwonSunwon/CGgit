#include "stdafx.h"

typedef class Camera
{
private:
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

public:
    Camera();

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

} Camera;

typedef class Object
{
protected:
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotate;

    GLuint vao;
    GLuint vbo;
    GLuint cbo;
    GLuint ebo;

    vector<float> vertices;
    vector<float> colors;
    vector<GLubyte> indices;

    glm::mat4 model;

public:
    Object();
    Object(vector<float> vertices, vector<float> colors);
    Object(vector<float> vertices, vector<float> colors, vector<GLubyte> indices);

    virtual void init();
    void initPos();
    void initBuffer();

    void setModelPos(vector<float> vertices);
    void setModelColor(vector<float> colors);
    void setModelIndices(vector<GLubyte> indices);

    void initModel(vector<float> vertices, vector<float> colors);
    void initModel(vector<float> vertices, vector<float> colors, vector<GLubyte> indices);

    virtual void render(GLuint shaderProgramID){};

    // Setters
    void setPos(glm::vec3 pos);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);

    void setScale(glm::vec3 scale);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);

    void setRotate(glm::vec3 rotate);
    void setRotateX(float x);
    void setRotateY(float y);
    void setRotateZ(float z);

    // Getters
    glm::vec3 getPos();
    glm::vec3 getScale();
    glm::vec3 getRotate();

} Object;

// Shader functions
void makeVertexShaders(char *file);
void makeFragmentShaders(char *file);
GLuint initShader(char *vertexFile, char *fragmentFile);

// void initVAO(GLuint &VAO);
// void initVBO_position(GLuint &VBO_position);
// void initVBO_color(GLuint &VBO_color);
// void initEBO(GLuint &EBO);

char *fileToBuf(char *fileName);