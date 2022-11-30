#include "sphere.h"

#ifndef __SPHERE_STATIC__
#define __SPHERE_STATIC__

GLint Sphere::object = -1;
vector<glm::vec3> Sphere::vertices;
vector<glm::vec3> Sphere::normals;

#endif // __SPHERE_STATIC__

Sphere::Sphere()
{
    if (object == -1)
    {
        object = objReader.loadObj("sphere.obj");
        vertices.resize(objReader.out_vertices.size());
        normals.resize(objReader.out_normals.size());
        for (int i = 0; i < objReader.out_vertices.size(); i++)
        {
            vertices[i] = objReader.out_vertices[i];
            normals[i] = objReader.out_normals[i];
        }
    }
}

void Sphere::render(GLuint ID)
{
    glUseProgram(ID);

    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, Sphere::object);
}

void Sphere::colorInit(glm::vec3 color)
{
    for (int i = 0; i < Sphere::vertices.size(); ++i)
    {
        colors.push_back(color.x);
        colors.push_back(color.y);
        colors.push_back(color.z);
    }

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(2);
}

void Sphere::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}
