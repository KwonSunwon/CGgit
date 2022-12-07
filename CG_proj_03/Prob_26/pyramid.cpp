#include "pyramid.h"

#include "stb_image.h"

Pyramid::Pyramid()
{
    const vector<float> &temp = {
        // front
        +0.0, +0.5, +0.0, 0, 1, 1, 0.5, 0, // 0
        -0.5, +0.0, +0.5, 0, 1, 1, 0, 1,   // 2
        +0.5, +0.0, +0.5, 0, 1, 1, 1, 1,   // 3

        // right
        +0.0, +0.5, +0.0, 1, 1, 0, 0.5, 0, // 0
        +0.5, +0.0, +0.5, 1, 1, 0, 0, 1,   // 2
        +0.5, +0.0, -0.5, 1, 1, 0, 1, 1,   // 3

        // left
        +0.0, +0.5, +0.0, -1, 1, 0, 0.5, 0, // 0
        -0.5, +0.0, -0.5, -1, 1, 0, 0, 1,   // 2
        -0.5, +0.0, +0.5, -1, 1, 0, 1, 1,   // 3

        // back
        +0.0, +0.5, +0.0, 0, 1, -1, 0.5, 0, // 0
        +0.5, +0.0, -0.5, 0, 1, -1, 0, 1,   // 4
        -0.5, +0.0, -0.5, 0, 1, -1, 1, 1,   // 1

        // bottom
        -0.5, +0.0, -0.5, 0, -1, 0, 0, 0, // 1
        +0.5, +0.0, -0.5, 0, -1, 0, 1, 0, // 4
        +0.5, +0.0, +0.5, 0, -1, 0, 1, 1, // 3
        +0.5, +0.0, +0.5, 0, -1, 0, 1, 1, // 3
        -0.5, +0.0, +0.5, 0, -1, 0, 0, 1, // 2
        -0.5, +0.0, -0.5, 0, -1, 0, 0, 0  // 1
    };

    for (int i = 0; i < temp.size(); i += 8)
    {
        vertices.push_back(glm::vec3(temp[i], temp[i + 1], temp[i + 2]));
        normals.push_back(glm::vec3(temp[i + 3], temp[i + 4], temp[i + 5]));
        uvs.push_back(glm::vec2(temp[i + 6], temp[i + 7]));
    }
    initPos();
}

void Pyramid::render(GLuint ID)
{
    glUseProgram(ID);

    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, glm::vec3(2, 3, 2));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(ID, "lightOn"), 1);

    glBindVertexArray(vao);

    int vertSize = vertices.size() / 6;

    for (int i = 0; i < 4; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[i]);
        glDrawArrays(GL_TRIANGLES, vertSize * i, vertSize);
    }

    glBindTexture(GL_TEXTURE_2D, textureID[4]);
    glDrawArrays(GL_TRIANGLES, vertSize * 4, vertSize * 2);
}

void Pyramid::initBuffer()
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

void Pyramid::initTexture()
{
    glGenTextures(5, textureID);

    for (int i = 0; i < 5; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    int width, height, nrChannels;
    unsigned char *data;

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    data = stbi_load("cat.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    data = stbi_load("cat1.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textureID[2]);
    data = stbi_load("cat2.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textureID[3]);
    data = stbi_load("img4.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textureID[4]);
    data = stbi_load("cat3.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void Pyramid::update()
{
    if (xAxisRotating)
        rotate.x += 0.5;
    if (yAxisRotating)
        rotate.y += 0.5;
}