#include "stdafx.h"
#include "shaders.h"

#include "camera.h"
#include "object.h"

#define BASE_SIZE 100.f
#define GRAVITY 1.f

#define PERSP 0
#define ORTHO 1

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> random_height(5.0, 20.0);
uniform_real_distribution<> random_step(0.0, 1.0);
uniform_real_distribution<> random_speed(0.1, 0.5);
uniform_real_distribution<> random_color(0.0, 1.0);

typedef class Base : public Object
{
public:
    void init(float color);
    void render(GLuint shaderProgramID) override;
    void update() override{};
} Base;

typedef class Pillar : public Object
{
private:
    float height;
    float step;

    float speed;

    bool isAnimating;
    bool isFall;
    glm::vec2 width;

    void fall();
    void move();

public:
    void init(int row, int col, int idx);
    void render(GLuint shaderProgramID) override;
    void update() override;

    void setSpeed(float speed) { this->speed = speed; }
    void setHeight(float height) { this->height = height; }
    void setAnimating(bool isAnimating) { this->isAnimating = isAnimating; }

    float getSpeed() { return this->speed; }
    float getHeight() { return this->height; }
    float getAnimating() { return this->isAnimating; }
} Pillar;

typedef class PillarManager
{
private:
    vector<Pillar> pillars;
    int row, col;

    bool isLowHeight;

public:
    void init(int row, int col);
    void render(GLuint shaderProgramID);
    void update();

    void makeMaze();
    void setAnimating(bool isAnimating);

    void setLowestHeight();

    void speedUp();
    void speedDown();
} PillarManager;

vector<int> _makeMaze(int row, int col);

void allObjectsInit();

PillarManager pillars;
Camera mainCamera;
Camera minimapCamera;

Base base;
Base base2;

int projectionMode = PERSP;
int test;
int _count = 1;

void update(int value);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid specialKeyboard(int key, int x, int y);

GLclampf g_color[4] = {0.f, 0.f, 0.f, 1.0f};
GLint width = 1200, height = 800;

// shader variables
GLuint shaderID;

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("M.M.M");
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    char vertexFile[] = "shader.vert";
    char fragmentFile[] = "shader.frag";
    shaderID = initShader(vertexFile, fragmentFile);

    allObjectsInit();

    update(0);

    cout << "o/p  : 투영 선택(직각/원근)\n";
    cout << "z/Z  : 원근 투영 시 z축으로 이동\n";
    cout << "m/M  : 육면체 기둥 위 아래로 애니메이션/정지\n";
    cout << "y/Y  : 바닥 y축 기준 회전\n";
    cout << "r    : 미로 제작\n";
    cout << "v    : 육면체 애니메이션 정지 및 낮은 높이로 변환\n";
    cout << "s    : 미로에 캐릭터 생성\n";
    cout << "Arrow: 캐릭터 이동\n";
    cout << "+/-  : 육면체 애니메이션 속도 증가/감소\n";
    cout << "1/3  : 1인칭/3인칭 시점 변환\n";
    cout << "c    : 초기화\n";
    cout << "q    : 프로그램 종료\n";

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 800, height);
    mainCamera.setCamera(shaderID, projectionMode);
    base.render(shaderID);
    base2.render(shaderID);
    // robot.render(shaderProgramID);
    pillars.render(shaderID);

    glViewport(800, 400, 400, 400);
    minimapCamera.setCamera(shaderID, 0);
    base.render(shaderID);
    base2.render(shaderID);
    // robot.render(shaderProgramID);
    pillars.render(shaderID);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, 800, 800);
}

GLvoid keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'o':
        projectionMode = ORTHO;
        break;
    case 'p':
        projectionMode = PERSP;
        break;

    // move camera
    // FIXME: 움직일 때 바닥이 사라졌다 나타났다 함
    case 'z':
        if (projectionMode == PERSP)
            mainCamera.setEye(mainCamera.getEye() + glm::vec3(0, 0, 1));
        break;
    case 'Z':
        if (projectionMode == PERSP)
            mainCamera.setEye(mainCamera.getEye() + glm::vec3(0, 0, -1));
        break;

    // pillar animation
    case 'm':
        pillars.setAnimating(true);
        break;
    case 'M':
        pillars.setAnimating(false);
        break;

    // y-axis rotation
    case 'y':
        break;
    case 'Y':
        break;

    // make maze
    case 'r':
        pillars.makeMaze();
        break;

    // make height low and stop animation
    case 'v':
        pillars.setLowestHeight();
        break;

    // show character
    case 's':
        break;

    // animation speed
    case '+':
        pillars.speedUp();
        break;
    case '-':
        pillars.speedDown();
        break;

    // change view mode
    case '1':
        break;
    case '3':
        break;

    // reset
    case 'c':
        break;

    // Exit
    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    // move character
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_DOWN:
        break;
    case GLUT_KEY_LEFT:
        break;
    case GLUT_KEY_RIGHT:
        break;
    }
    glutPostRedisplay();
}

void allObjectsInit()
{
    // Camera init
    mainCamera.setEye(glm::vec3(0.f, 100.0f, -120.f));
    mainCamera.setTarget(glm::vec3(0.f, 0.f, 0.f));
    mainCamera.setzFar(500.f);
    mainCamera.setPitch(-45.f);
    mainCamera.setYaw(90.f);

    mainCamera.setLeft(-100.f);
    mainCamera.setRight(100.f);
    mainCamera.setBottom(-100.f);
    mainCamera.setTop(100.f);

    // Minimap init
    minimapCamera.setEye(glm::vec3(0.f, 120.0f, 0.f));
    minimapCamera.setTarget(glm::vec3(0.f, 0.f, 0.f));
    minimapCamera.setzFar(500.f);
    minimapCamera.setPitch(-90.f);

    base.init(1.f);
    base2.init(.5f);
    base2.setPosY(-.2f);
    base2.setScale(glm::vec3(1000.f, .1f, 1000.f));

    pillars.init(25, 25);
}

void update(int value)
{
    pillars.update();
    glutTimerFunc(1000 / 60, update, 0);
    glutPostRedisplay();
}

#pragma endregion

#pragma region BaseClass

void Base::init(float color)
{
    const vector<float> baseVertices = {
        -55.f, 0.f, -55.f,
        55.f, 0.f, -55.f,
        55.f, 0.f, 55.f,
        -55.f, 0.f, 55.f};
    const vector<GLubyte> baseIndices = {
        0, 2, 1,
        0, 3, 2};
    vector<float> baseColor;
    for (int i = 0; i < 4; i++)
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

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)0);
}

#pragma endregion

#pragma region PillarClass

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

    pos.x = -BASE_SIZE / 2.f + width.x / 2.f + width.x * (idx % row);
    pos.z = -BASE_SIZE / 2.f + width.y / 2.f + width.y * (idx / row);

    pos.y = 100.f;

    height = random_height(gen);
    step = 0;
    isFall = true;

    speed = random_speed(gen);

    isAnimating = true;
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

#pragma endregion

#pragma region MakeMaze

#define LEFT 0b0001
#define RIGHT 0b0010
#define UP 0b0100
#define DOWN 0b1000

vector<int> _makeMaze(int row, int col)
{
    // 조건 1 : 한 방향으로 최대 8칸 이동
    // 조건 2 : 경로를 좌우상하로 최소 한 번 이동
    // 조건 3 : 한 번 지나갔던 길을 지나면 가장 마지막의 값으로 출력

    srand(time(NULL));

    int count = 1;
    int xPos = 0;
    int yPos = 0;
    int direction = RIGHT;
    int lastDirection = -1;

    int leftCnt = 0;
    int upCnt = 0;

    int conditionCount = 0;
    int conditionDirection = 0;

    bool isSafe;
    bool isSameDirection;

    vector<int> maze(row * col, 0);

    while (1)
    {
        switch (direction)
        {
        case RIGHT:
            ++yPos;
            conditionDirection |= RIGHT;
            break;

        case LEFT:
            --yPos;
            conditionDirection |= LEFT;
            break;

        case UP:
            --xPos;
            conditionDirection |= UP;
            break;

        case DOWN:
            ++xPos;
            conditionDirection |= DOWN;
            break;
        }

        if (xPos < 0 || xPos > col - 1 || yPos < 0 || yPos > row - 1)
        {
            vector<int> empty;
            return empty;
        }

        maze[yPos * row + xPos] = ++count;
        lastDirection = direction;
        if (xPos == col - 1 && yPos == row - 1 && conditionDirection == 0b1111)
            break;
        ++conditionCount;

        if (rand() % 10 == 0 || conditionCount == 7)
        {
            switch (direction)
            {
            case RIGHT:
                if (rand() % 4 == 0)
                    direction = UP;
                else
                    direction = DOWN;

                break;

            case LEFT:
                if (rand() % 4 == 0)
                    direction = UP;
                else
                    direction = DOWN;

                break;

            case UP:
                if (rand() % 4 == 0)
                    direction = LEFT;
                else
                    direction = RIGHT;
                break;

            case DOWN:
                if (rand() % 4 == 0)
                    direction = LEFT;
                else
                    direction = RIGHT;
                break;
            }
        }

        switch (direction)
        {
        case RIGHT:
            if (yPos + 1 >= row)
            {
                switch (lastDirection)
                {
                case RIGHT:
                    direction = DOWN;
                    break;
                case DOWN:
                case UP:
                    direction = LEFT;
                    break;
                }
            }
            break;
        case LEFT:
            if (yPos - 1 <= -1)
            {
                switch (lastDirection)
                {
                case LEFT:
                    direction = DOWN;
                    break;
                case DOWN:
                case UP:
                    direction = RIGHT;
                    break;
                }
            }
            break;
        case UP:
            if (xPos - 1 <= -1)
            {
                switch (lastDirection)
                {
                case RIGHT:
                case LEFT:
                    direction = DOWN;
                    break;
                case UP:
                    if (yPos + 1 >= row)
                        direction = LEFT;
                    else
                        direction = RIGHT;
                    break;
                }
            }
            break;
        case DOWN:
            if (xPos + 1 >= col)
            {
                switch (lastDirection)
                {
                case RIGHT:
                case LEFT:
                    direction = UP;
                    break;
                case DOWN:
                    direction = RIGHT;
                    break;
                }
            }
            break;
        }

        if (conditionCount == 7)
            conditionCount = 0;
    }

    return maze;
}

#pragma endregion
