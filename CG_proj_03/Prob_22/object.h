#include "stdafx.h"

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
    virtual void initBuffer();

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