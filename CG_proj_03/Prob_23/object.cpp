// Object Class
#include "object.h"

// Not using an Obj file
Object::Object()
{
    initPos();
}
Object::Object(vector<float> vertices, vector<float> colors)
{
    initPos();
    initModel(vertices, colors);
}
Object::Object(vector<float> vertices, vector<float> colors, vector<GLubyte> indices)
{
    initPos();
    initModel(vertices, colors, indices);
}

// Using an Obj file
// Object::Object(const char *filename)
// {
//     object = objReader.loadObj(filename);
// }

void Object::initPos()
{
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Object::setPos(glm::vec3 pos) { this->pos = pos; }
void Object::setPosX(float x) { this->pos.x = x; }
void Object::setPosY(float y) { this->pos.y = y; }
void Object::setPosZ(float z) { this->pos.z = z; }

void Object::setScale(glm::vec3 scale) { this->scale = scale; }
void Object::setScaleX(float x) { this->scale.x = x; }
void Object::setScaleY(float y) { this->scale.y = y; }
void Object::setScaleZ(float z) { this->scale.z = z; }

void Object::setRotate(glm::vec3 rotate) { this->rotate = rotate; }
void Object::setRotateX(float x) { this->rotate.x = x; }
void Object::setRotateY(float y) { this->rotate.y = y; }
void Object::setRotateZ(float z) { this->rotate.z = z; }

void Object::setRevolution(glm::vec3 revolution) { this->revolution = revolution; }
void Object::setRevolutionX(float x) { this->revolution.x = x; }
void Object::setRevolutionY(float y) { this->revolution.y = y; }
void Object::setRevolutionZ(float z) { this->revolution.z = z; }

glm::vec3 Object::getPos() { return pos; }
glm::vec3 Object::getScale() { return scale; }
glm::vec3 Object::getRotate() { return rotate; }
glm::vec3 Object::getRevolution() { return revolution; }
int Object::getId() { return id; }

void Object::setModelPos(vector<float> vertices)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        this->_vertices.push_back(vertices[i]);
    }
}
void Object::setModelColor(vector<float> colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        this->colors.push_back(colors[i]);
    }
}
void Object::setModelIndices(vector<GLubyte> indices)
{
    for (int i = 0; i < indices.size(); i++)
    {
        this->_indices.push_back(indices[i]);
    }
}

void Object::initModel(vector<float> vertices, vector<float> colors)
{
    setModelPos(vertices);
    setModelColor(colors);
}

void Object::initModel(vector<float> vertices, vector<float> colors, vector<GLubyte> indices)
{
    setModelPos(vertices);
    setModelColor(colors);
    setModelIndices(indices);
}

void Object::init()
{
    initPos();
    _initBuffer();
}

void Object::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &nbo);

    // vertices
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, objReader.out_vertices.size() * sizeof(glm::vec3), &objReader.out_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, objReader.out_normals.size() * sizeof(glm::vec3), &objReader.out_normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
}

void Object::_initBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    if (!_indices.empty())
    {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size(), &_indices[0], GL_STATIC_DRAW);
    }
}
