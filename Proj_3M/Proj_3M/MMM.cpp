#include "src/stdafx.h"
#include "src/shaders.h"

#include "src/camera.h"
#include "src/object.h"

#include "src/pillars.h"
#include "src/base.h"
#include "src/player.h"
#include "src/firstCamera.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid specialKeyboard(int key, int x, int y);
GLvoid keyUp(int key, int x, int y);

GLclampf g_color[4] = {0.f, 0.f, 0.f, 1.0f};
GLint width = 1200, height = 800;

int projectionMode = PERSP;
bool isShowPlayer = false;
int viewMode = THIRD;

void initAllObjects();
void resetAllObjects();
void update(int value);

void checkCollision();
bool isCollide(glm::vec4 a, glm::vec4 b);

int row, col;

// shader variables
GLuint shaderID;

int minimapSize = 200;

Camera mainCamera(width, height);
Camera minimapCamera(minimapSize, minimapSize);
FirstCamera firstCamera(width, height);

Base base;
Base base2;
PillarManager pillars;
Player player;

void main(int argc, char **argv)
{
    bool isWrongValue = true;

    while (isWrongValue)
    {
        system("cls");
        if (isWrongValue)
            cout << "5 ~ 25 사이의 값 입력\n";
        cout << "크기 입력 (행 / 열) : ";
        cin >> row >> col;
        if (row < 5 || col < 5 || row > 25 || col > 25)
            isWrongValue = true;
        else
            isWrongValue = false;
    }

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    char vertexFile[] = "res/shader.vert";
    char fragmentFile[] = "res/shader.frag";
    shaderID = initShader(vertexFile, fragmentFile);

    initAllObjects();
    update(0);

    system("cls");
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
    glutSpecialUpFunc(keyUp);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width, height);
    if (viewMode == THIRD)
        mainCamera.setCamera(shaderID, projectionMode);
    else
        firstCamera.setCamera(shaderID);
    base.render(shaderID);
    pillars.render(shaderID);
    if (isShowPlayer)
        player.render(shaderID);

    glViewport(width - minimapSize, height - minimapSize, minimapSize, minimapSize);
    glDepthFunc(GL_ALWAYS);
    minimapCamera.setCamera(shaderID, 0);
    base2.render(shaderID);

    glDepthFunc(GL_LESS);
    base.render(shaderID);
    pillars.render(shaderID);
    if (isShowPlayer)
        player.render(shaderID);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
}

GLvoid keyboard(unsigned char key, int x, int y)
{
    glm::vec4 test;

    switch (key)
    {
    case 'o':
        projectionMode = ORTHO;
        break;
    case 'p':
        projectionMode = PERSP;
        break;

    // move camera
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
        if (viewMode == THIRD)
            if (!mainCamera.getRotating())
                mainCamera.setRotating(1);
            else
                mainCamera.setRotating(0);
        break;
    case 'Y':
        if (viewMode == THIRD)
            if (!mainCamera.getRotating())
                mainCamera.setRotating(-1);
            else
                mainCamera.setRotating(0);
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
        isShowPlayer = true;
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
        viewMode = FIRST;
        player.setViewMode(viewMode);
        break;
    case '3':
        viewMode = THIRD;
        player.setViewMode(viewMode);
        break;

    // reset
    case 'c':
        resetAllObjects();
        break;

    // Exit
    case 'Q':
    case 'q':
        exit(0);
        break;
    }
}

GLvoid specialKeyboard(int key, int x, int y)
{
    player.keyInput(key, 0);
}

GLvoid keyUp(int key, int x, int y)
{
    player.keyInput(key, 1);
}

void initAllObjects()
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

    minimapCamera.setUp(glm::vec3(0.f, 0.f, 1.f));

    minimapCamera.setzFar(500.f);
    minimapCamera.setPitch(-90.f);

    base.init(1.f);
    base.setPosY(-100.f);

    base2.init(.5f);
    base2.setPosY(-150.f);
    base2.setScale(glm::vec3(500.f, 1.f, 500.f));

    pillars.init(row, col);

    player.init();

    firstCamera.init(player.getPos());
}

void resetAllObjects()
{
    mainCamera.setEye(glm::vec3(0.f, 100.0f, -120.f));
    mainCamera.setTarget(glm::vec3(0.f, 0.f, 0.f));
    mainCamera.setPitch(-45.f);
    mainCamera.setYaw(90.f);

    player.reset();
    pillars.reset();

    isShowPlayer = false;
    viewMode = THIRD;
    projectionMode = PERSP;
}

void update(int value)
{
    pillars.update();

    if (isShowPlayer)
    {
        player.update();
        checkCollision();
    }

    mainCamera.update();
    firstCamera.update(player.getPos(), player.getDirection());

    glutTimerFunc(1000 / 60, update, 0);
    glutPostRedisplay();
}

bool isCollide(glm::vec4 a, glm::vec4 b)
{
    if (a.x > b.y || a.y < b.x || a.z < b.w || a.w > b.z)
        return false;
    return true;
}

void checkCollision()
{
    glm::vec4 playerRect = player.getBound();

    if (playerRect.x < -55.f || playerRect.y > 55.f || playerRect.z > 55.f || playerRect.w < -55.f)
    {
        player.handleCollision();
        return;
    }
    for (int i = 0; i < pillars.getPillarCount(); i++)
    {
        if (isCollide(playerRect, pillars.getPillar(i).getBound()) && pillars.getPillar(i).getPos().y > -20.f)
        {
            player.handleCollision();
            break;
        }
    }
}