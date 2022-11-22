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
uniform_int_distribution<> random_maze(0, 2);

typedef class Base : public Object
{
public:
    void init(float color);
    void render(GLuint shaderProgramID) override;
    void update() override{};
} Base;

typedef class Robot : public Object
{
private:
    float headHeight;
    float bodyHeight;
    glm::vec3 armRightPos;
    glm::vec3 armLeftPos;
    glm::vec3 legRightPos;
    glm::vec3 legLeftPos;

    float armAngle;
    float legAngle;

    void drawHead(GLuint ID);
    void drawBody(GLuint ID);
    void drawArmRight(GLuint ID);
    void drawArmLeft(GLuint ID);
    void drawLeg(GLuint ID);

public:
    void init() override;
    void render(GLuint shaderProgramID) override;
} Robot;

typedef class Pillar : public Object
{
private:
    float height;
    float step;

    float speed;

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
} Pillar;

typedef class PillarManager
{
private:
    vector<Pillar> pillars;
    int row, col;

public:
    void init(int row, int col);
    void render(GLuint shaderProgramID);
    void update();

    void makeMaze();
} PillarManager;

void allObjectsInit();

PillarManager pillars;
Camera mainCamera;
Camera minimapCamera;

Robot robot;
Base base;
Base base2;

int projectionMode = PERSP;

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

    // y-axis rotation
    case 'y':
        break;
    case 'Y':
        break;

    // make maze
    case 'r':
        pillars.makeMaze();
        break;

    // show character
    case 's':
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
    mainCamera.setEye(glm::vec3(0.f, 100.0f, 120.f));
    mainCamera.setTarget(glm::vec3(0.f, 0.f, 0.f));
    mainCamera.setzFar(500.f);
    mainCamera.setPitch(-45.f);

    mainCamera.setLeft(-100.f);
    mainCamera.setRight(100.f);
    mainCamera.setBottom(-100.f);
    mainCamera.setTop(100.f);

    // Minimap init
    minimapCamera.setEye(glm::vec3(0.f, 120.0f, 0.f));
    minimapCamera.setTarget(glm::vec3(0.f, 0.f, 0.f));
    minimapCamera.setzFar(500.f);
    minimapCamera.setPitch(-90.f);

    robot.init();
    base.init(1.f);
    base2.init(.5f);
    base2.setPosY(-.2f);
    base2.setScale(glm::vec3(1000.f, .1f, 1000.f));

    pillars.init(20, 20);
}

void update(int value)
{
    pillars.update();
    glutTimerFunc(1000 / 60, update, 0);
    glutPostRedisplay();
}

#pragma region RobotClass

void Robot::init()
{
    const std::vector<float> vertices = {
        // Head
        -1.f, 2.f, -.5f,
        -1.f, 0.f, -.5f,
        1.f, 0.f, -.5f,
        1.f, 2.f, -.5f,

        1.f, 2.f, .5f,
        1.f, 0.f, .5f,
        -1.f, 0.f, .5f,
        -1.f, 2.f, .5f,

        // Head - Nose
        -.2f, .3f, .6f,
        -.2f, 1.f, .6f,
        .2f, 1.f, .6f,
        .2f, .3f, .6f,

        // Body
        -1.5f, 4.f, -1.f,
        -1.5f, 0.f, -1.f,
        1.5f, 0.f, -1.f,
        1.5f, 4.f, -1.f,

        1.5f, 4.f, 1.f,
        1.5f, 0.f, 1.f,
        -1.5f, 0.f, 1.f,
        -1.5f, 4.f, 1.f,

        // Arm Right
        -.5f, 3.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 3.f, -.5f,

        .5f, 3.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 3.f, .5f,

        // Arm Left
        -.5f, 3.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 3.f, -.5f,

        .5f, 3.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 3.f, .5f,

        // Leg
        -.5f, 4.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 4.f, -.5f,

        .5f, 4.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 4.f, .5f};

    const std::vector<GLubyte> indices = {
        // Head
        0, 2, 1, 0, 3, 2, // Front
        4, 6, 5, 4, 7, 6, // Back
        7, 1, 6, 7, 0, 1, // Left
        3, 5, 2, 3, 4, 5, // Right
        7, 3, 0, 7, 4, 3, // Top
        1, 5, 6, 1, 2, 5, // Bottom

        // Nose
        8, 9, 10, 8, 10, 11,

        // Body
        12, 14, 13, 12, 15, 14, // Front
        16, 18, 17, 16, 19, 18, // Back
        19, 13, 18, 19, 12, 13, // Left
        15, 17, 14, 15, 16, 17, // Right
        19, 15, 12, 19, 16, 15, // Top
        13, 17, 18, 13, 14, 17, // Bottom

        // Arm Right
        20, 22, 21, 20, 23, 22, // Front
        24, 26, 25, 24, 27, 26, // Back
        27, 21, 26, 27, 20, 21, // Left
        23, 25, 22, 23, 24, 25, // Right
        27, 23, 20, 27, 24, 23, // Top
        21, 25, 26, 21, 22, 25, // Bottom

        // Arm Left
        28, 30, 29, 28, 31, 30, // Front
        32, 34, 33, 32, 35, 34, // Back
        35, 29, 34, 35, 28, 29, // Left
        31, 33, 30, 31, 32, 33, // Right
        35, 31, 28, 35, 32, 31, // Top
        29, 33, 34, 29, 30, 33, // Bottom

        // Leg
        36, 38, 37, 36, 39, 38, // Front
        40, 42, 41, 40, 43, 42, // Back
        43, 37, 42, 43, 36, 37, // Left
        39, 41, 38, 39, 40, 41, // Right
        43, 39, 36, 43, 40, 39, // Top
        37, 41, 42, 37, 38, 41  // Bottom
    };

    std::vector<float> colors;

    // Head
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(0.f);
        colors.push_back(.4f);
        colors.push_back(.8f);
    }
    // Nose
    for (int i = 0; i < 4; ++i)
    {
        colors.push_back(.5f);
        colors.push_back(.5f);
        colors.push_back(0.f);
    }
    // Body
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(.8f);
        colors.push_back(.4f);
        colors.push_back(0.f);
    }
    // Arm Right
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(0.f);
        colors.push_back(1.f);
        colors.push_back(0.f);
    }
    // Arm Left
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(1.f);
        colors.push_back(0.f);
        colors.push_back(0.f);
    }
    // Leg
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(0.f);
        colors.push_back(0.f);
        colors.push_back(1.f);
    }

    initModel(vertices, colors, indices);
    initBuffer();

    initPos();
    scale = glm::vec3(.1f, .1f, .1f);

    headHeight = 8.f;
    bodyHeight = 4.f;
    armRightPos = glm::vec3(-1.5f, 4.f, 0.f);
    armLeftPos = glm::vec3(1.5f, 4.f, 0.f);
    legRightPos = glm::vec3(-.5f, 0.f, 0.f);
    legLeftPos = glm::vec3(.5f, 0.f, 0.f);

    rotate.y = 10.f;
}

void Robot::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, scale);

    drawHead(shaderProgramID);
    drawBody(shaderProgramID);
    drawArmRight(shaderProgramID);
    drawArmLeft(shaderProgramID);
    drawLeg(shaderProgramID);
}

void Robot::drawHead(GLuint ID)
{
    glm::mat4 headModel = glm::mat4(1.f);
    headModel = glm::translate(headModel, glm::vec3(0.f, headHeight, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * headModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);

    // Nose
    glm::mat4 noseModel = glm::mat4(1.f);
    noseModel = glm::translate(noseModel, glm::vec3(0.f, headHeight, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * noseModel));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)36);
}

void Robot::drawBody(GLuint ID)
{
    glm::mat4 bodyModel = glm::mat4(1.f);
    bodyModel = glm::translate(bodyModel, glm::vec3(0.f, bodyHeight, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * bodyModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)42);
}

void Robot::drawArmLeft(GLuint ID)
{
    glm::mat4 armLeftModel = glm::mat4(1.f);
    armLeftModel = glm::translate(armLeftModel, armLeftPos);
    armLeftModel = glm::rotate(armLeftModel, glm::radians(-armAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * armLeftModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)78);
}

void Robot::drawArmRight(GLuint ID)
{
    glm::mat4 armRightModel = glm::mat4(1.f);
    armRightModel = glm::translate(armRightModel, armRightPos);
    armRightModel = glm::rotate(armRightModel, glm::radians(armAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * armRightModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)114);
}

void Robot::drawLeg(GLuint ID)
{
    glm::mat4 legModel = glm::mat4(1.f);
    legModel = glm::translate(legModel, legRightPos);
    legModel = glm::rotate(legModel, glm::radians(legAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * legModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)150);

    legModel = glm::mat4(1.f);
    legModel = glm::translate(legModel, legLeftPos);
    legModel = glm::rotate(legModel, glm::radians(-legAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * legModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)150);
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
    row = this->row;
    col = this->col;

    vector<int> maze;
    for (int y = 0; y < row; y++)
    {
        for (int x = 0; x < col; x++)
        {
            if (x % 2 == 0 || y % 2 == 0)
                maze.push_back(1);
            else
                maze.push_back(0);
        }
    }

    for (int y = 0; y < row; y++)
    {
        int count = 1;
        for (int x = 0; x < col; x++)
        {
            if (x % 2 == 0 || y % 2 == 0)
                continue;

            if (x == col - 2 && y == row - 2)
                continue;

            if (x == col - 2)
            {
                maze[(y + 1) * col + x] = 0;
                continue;
            }

            if (y == row - 2)
            {
                maze[y * col + x + 1] = 0;
                continue;
            }

            int rand = random_maze(gen);
            if (rand == 0)
            {
                uniform_int_distribution<int> random_idx(0, count);
                maze[(y + 1) * col + x - random_idx(gen) * 2] = 0;
                count = 1;
                break;
            }
            else
            {
                maze[y * col + x + 1] = 0;
                count += 1;
                break;
            }
        }
    }
}

#pragma endregion
