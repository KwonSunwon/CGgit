#include "cube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cube::Cube()
{
    // object = objReader.loadObj("cube.obj");
    // vertices.resize(objReader.out_vertices.size());
    // normals.resize(objReader.out_normals.size());
    // uvs.resize(objReader.out_uvs.size());

    // for (int i = 0; i < objReader.out_vertices.size(); i++)
    // {
    //     vertices[i] = objReader.out_vertices[i];
    //     normals[i] = objReader.out_normals[i];
    //     uvs[i] = objReader.out_uvs[i];
    // }

    const vector<float> &temp = {
        0, 0, 0, 0, 0, -1, 0, 1,
        0, 1, 0, 0, 0, -1, 0, 0,
        1, 1, 0, 0, 0, -1, 1, 0,
        0, 0, 0, 0, 0, -1, 0, 1,
        1, 1, 0, 0, 0, -1, 1, 0,
        1, 0, 0, 0, 0, -1, 1, 1,

        0, 0, 1, -1, 0, 0, 0, 1,
        0, 1, 1, -1, 0, 0, 0, 0,
        0, 1, 0, -1, 0, 0, 1, 0,
        0, 0, 1, -1, 0, 0, 0, 1,
        0, 1, 0, -1, 0, 0, 1, 0,
        0, 0, 0, -1, 0, 0, 1, 1,

        1, 0, 0, 1, 0, 0, 0, 1,
        1, 1, 0, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 1, 0,
        1, 0, 0, 1, 0, 0, 0, 1,
        1, 1, 1, 1, 0, 0, 1, 0,
        1, 0, 1, 1, 0, 0, 1, 1,

        1, 0, 1, 0, 0, 1, 0, 1,
        1, 1, 1, 0, 0, 1, 0, 0,
        0, 1, 1, 0, 0, 1, 1, 0,
        1, 0, 1, 0, 0, 1, 0, 1,
        0, 1, 1, 0, 0, 1, 1, 0,
        0, 0, 1, 0, 0, 1, 1, 1,

        0, 1, 0, 0, 1, 0, 0, 1,
        0, 1, 1, 0, 1, 0, 0, 0,
        1, 1, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 0, 1, 0, 0, 1,
        1, 1, 1, 0, 1, 0, 1, 0,
        1, 1, 0, 0, 1, 0, 1, 1,

        0, 0, 1, 0, -1, 0, 0, 1,
        0, 0, 0, 0, -1, 0, 0, 0,
        1, 0, 0, 0, -1, 0, 1, 0,
        0, 0, 1, 0, -1, 0, 0, 1,
        1, 0, 0, 0, -1, 0, 1, 0,
        1, 0, 1, 0, -1, 0, 1, 1};

    for (int i = 0; i < temp.size(); i += 8)
    {
        vertices.push_back(glm::vec3(temp[i], temp[i + 1], temp[i + 2]));
        normals.push_back(glm::vec3(temp[i + 3], temp[i + 4], temp[i + 5]));
        uvs.push_back(glm::vec2(temp[i + 6], temp[i + 7]));
    }
    initPos();
    pos = glm::vec3(-1, -1, -1);
}

void Cube::render(GLuint ID)
{
    glUseProgram(ID);

    model = glm::mat4(1.0);
    model = glm::translate(model, pos);
    model = glm::translate(model, glm::vec3(1, 1, 1));
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-1, -1, -1));
    model = glm::scale(model, glm::vec3(2, 2, 2));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(ID, "lightOn"), 1);

    glBindVertexArray(vao);

    int vertSize = vertices.size() / 6;

    for (int i = 0; i < 6; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[i]);
        glDrawArrays(GL_TRIANGLES, vertSize * i, vertSize);
    }
}

void Cube::initBuffer()
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

void Cube::initTexture()
{
    glGenTextures(6, textureID);

    for (int i = 0; i < 6; i++)
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

    glBindTexture(GL_TEXTURE_2D, textureID[5]);
    data = stbi_load("cat5.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

void Cube::rotation(glm::vec3 rotate)
{
    this->rotate += rotate;
}

void Cube::update()
{
    if (xAxisRotating)
        rotate.x += 0.5;
    if (yAxisRotating)
        rotate.y += 0.5;
}