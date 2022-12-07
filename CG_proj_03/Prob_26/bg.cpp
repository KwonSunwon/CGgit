#include "bg.h"

#include "stb_image.h"

BG::BG()
{
    const vector<float> &temp = {
        1, 0, -10, 0, 0, 1, 0, 1,
        1, 1, -10, 0, 0, 1, 0, 0,
        0, 1, -10, 0, 0, 1, 1, 0,
        1, 0, -10, 0, 0, 1, 0, 1,
        0, 1, -10, 0, 0, 1, 1, 0,
        0, 0, -10, 0, 0, 1, 1, 1};

    for (int i = 0; i < temp.size(); i += 8)
    {
        vertices.push_back(glm::vec3(temp[i], temp[i + 1], temp[i + 2]));
        normals.push_back(glm::vec3(temp[i + 3], temp[i + 4], temp[i + 5]));
        uvs.push_back(glm::vec2(temp[i + 6], temp[i + 7]));
    }
    initPos();
    pos = glm::vec3(-25, -25, 0);
    scale = glm::vec3(50, 50, 1);
}

void BG::render(GLuint ID)
{
    glUseProgram(ID);

    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(ID, "lightOn"), 0);

    glBindVertexArray(vao);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void BG::initBuffer()
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

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
}

void BG::initTexture()
{
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data;

    glBindTexture(GL_TEXTURE_2D, textureID);
    data = stbi_load("sky.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
