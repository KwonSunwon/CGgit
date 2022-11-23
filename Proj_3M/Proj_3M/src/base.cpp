#include "base.h"

void Base::init(float color)
{
    const vector<float> baseVertices = {
        -55.f, 100.f, -55.f,
        -55.f, 0.f, -55.f,
        55.f, 0.f, -55.f,
        55.f, 100.f, -55.f,

        55.f, 100.f, 55.f,
        55.f, 0.f, 55.f,
        -55.f, 0.f, 55.f,
        -55.f, 100.f, 55.f};
    const vector<GLubyte> baseIndices = {
        0, 2, 1, 0, 3, 2,
        4, 6, 5, 4, 7, 6,
        7, 1, 6, 7, 0, 1,
        3, 5, 2, 3, 4, 5,
        7, 3, 0, 7, 4, 3,
        1, 5, 6, 1, 2, 5};
    vector<float> baseColor;
    for (int i = 0; i < 8; i++)
    {
        baseColor.push_back(color - .1f);
        baseColor.push_back(color);
        baseColor.push_back(color);
    }

    initModel(baseVertices, baseColor, baseIndices);
    initBuffer();

    initPos();
}

void Base::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
}