#include "pillars.h"
#include "maze.h"

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> random_height(5.0, 20.0);
uniform_real_distribution<> random_step(0.0, 1.0);
uniform_real_distribution<> random_speed(0.1, 0.5);
uniform_real_distribution<> random_color(0.2, 1.0);

void Pillar::init(int row, int col, int idx)
{
    const vector<float> vertices = {
        -.5f, 2.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 2.f, -.5f,

        .5f, 2.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 2.f, .5f};
    const vector<GLubyte> indices = {
        0, 2, 1, 0, 3, 2, // Front
        4, 6, 5, 4, 7, 6, // Back
        7, 1, 6, 7, 0, 1, // Left
        3, 5, 2, 3, 4, 5, // Right
        7, 3, 0, 7, 4, 3, // Top
        1, 5, 6, 1, 2, 5  // Bottom
    };

    float color = random_color(gen);

    vector<float> pillarColor;
    for (int i = 0; i < 8; i++)
    {
        pillarColor.push_back(color);
        pillarColor.push_back(color);
        pillarColor.push_back(color);
    }

    initModel(vertices, pillarColor, indices);
    initBuffer();

    initPos();

    width.x = BASE_SIZE / (float)row;
    width.y = BASE_SIZE / (float)col;

    scale.x = width.x;
    scale.z = width.y;

    pos.x = -BASE_SIZE / 2.f + width.x / 2.f + width.x * (idx % row);
    pos.z = -BASE_SIZE / 2.f + width.y / 2.f + width.y * (idx / row);

    pos.y = 100.f;

    height = random_height(gen);
    scale.y = height;

    step = 0;
    isFall = true;

    speed = random_speed(gen);

    isAnimating = true;
}

void Pillar::reset()
{
    pos.y = 100.f;
    step = 0;
    isFall = true;
    isAnimating = true;

    height = random_height(gen);
    scale.y = height;
    speed = random_speed(gen);
}

void Pillar::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(width.x, height, width.y));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
}

void Pillar::update()
{
    if (!isAnimating)
        return;

    if (isFall)
        fall();
    else
        move();
}

void Pillar::fall()
{
    pos.y += step * speed;
    if (pos.y <= 0.f)
    {
        step = 1;
        isFall = false;
    }
    else
        step -= GRAVITY;
}

void Pillar::move()
{
    pos.y += step * speed;
    if (pos.y < -height)
    {
        step = 1;
    }
    else if (pos.y > 0.f)
    {
        step = -1;
    }
}

void PillarManager::init(int row, int col)
{
    isLowHeight = false;

    this->row = row;
    this->col = col;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            Pillar pillar;
            pillar.init(row, col, i * col + j);
            pillars.push_back(pillar);
        }
    }
}

void PillarManager::render(GLuint shaderProgramID)
{
    for (int i = 0; i < pillars.size(); i++)
    {
        pillars[i].render(shaderProgramID);
    }
}

void PillarManager::update()
{
    for (int i = 0; i < pillars.size(); i++)
    {
        pillars[i].update();
    }
}

void PillarManager::makeMaze()
{
    vector<int> maze;

    do
    {
        maze = _makeMaze(row, col);
    } while (maze.size() == 0);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (maze[i * col + j] != 0)
            {
                pillars[i * col + j].setPosY(-100);
                pillars[i * col + j].setSpeed(0);
                pillars[i * col + j].setAnimating(false);
            }
        }
    }
}

void PillarManager::setAnimating(bool isAnimating)
{
    for (int i = 0; i < pillars.size(); i++)
    {
        pillars[i].setAnimating(isAnimating);
    }
}

void PillarManager::setLowestHeight()
{
    if (!isLowHeight)
    {
        for (int i = 0; i < pillars.size(); i++)
        {
            if (pillars[i].getAnimating())
            {
                pillars[i].setPosY(0);
                pillars[i].setHeight(5.f);
            }
            pillars[i].setAnimating(false);
        }
        isLowHeight = true;
    }
    else
    {
        for (int i = 0; i < pillars.size(); i++)
        {
            if (pillars[i].getPos().y == 0)
            {
                pillars[i].setHeight(random_height(gen));
                pillars[i].setSpeed(random_speed(gen));
                pillars[i].setAnimating(true);
            }
        }
        isLowHeight = false;
    }
}

void PillarManager::speedUp()
{
    for (int i = 0; i < pillars.size(); i++)
    {
        pillars[i].setSpeed(pillars[i].getSpeed() * 1.1f);
    }
}

void PillarManager::speedDown()
{
    for (int i = 0; i < pillars.size(); i++)
    {
        pillars[i].setSpeed(pillars[i].getSpeed() * 0.9f);
    }
}

void PillarManager::reset()
{
    isLowHeight = false;
    for (int i = 0; i < pillars.size(); i++)
    {
        pillars[i].reset();
    }
}