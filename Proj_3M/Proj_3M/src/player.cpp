#include "player.h"

void Player::init()
{
    const vector<float> vertices = {
        -.5f, 1.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 1.f, -.5f,

        .5f, 1.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 1.f, .5f};

    const vector<GLubyte> indices = {
        0, 2, 1, 0, 3, 2, // Front
        4, 6, 5, 4, 7, 6, // Back
        7, 1, 6, 7, 0, 1, // Left
        3, 5, 2, 3, 4, 5, // Right
        7, 3, 0, 7, 4, 3, // Top
        1, 5, 6, 1, 2, 5  // Bottom
    };

    vector<float> playerColor;
    for (int i = 0; i < 8; i++)
    {
        playerColor.push_back(0.8f);
        playerColor.push_back(0.5f);
        playerColor.push_back(0.1f);
    }

    initModel(vertices, playerColor, indices);
    initBuffer();

    initPos();

    scale = glm::vec3(3.f, 3.f, 3.f);
    pos = glm::vec3(50.f, 0.f, 50.f);
}

void Player::render(GLuint ID)
{
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.f, 0.f, 1.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}

void Player::update()
{
    prevPos = pos;
    move();
}

void Player::move()
{
    pos.x += speed * dir_x;
    pos.z += speed * dir_z;
}

void Player::keyInput(unsigned int key, int type)
{
    if (type == 0) // key down
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            dir_z = 1;
            break;
        case GLUT_KEY_DOWN:
            dir_z = -1;
            break;
        case GLUT_KEY_LEFT:
            dir_x = 1;
            break;
        case GLUT_KEY_RIGHT:
            dir_x = -1;
            break;
        }
    }
    else if (type == 1) // key up
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            dir_z = 0;
            break;
        case GLUT_KEY_DOWN:
            dir_z = 0;
            break;
        case GLUT_KEY_LEFT:
            dir_x = 0;
            break;
        case GLUT_KEY_RIGHT:
            dir_x = 0;
            break;
        }
    }
}

void Player::handleCollision()
{
    pos = prevPos;
}