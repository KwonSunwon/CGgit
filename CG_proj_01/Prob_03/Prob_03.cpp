#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct _Rect
{
    GLfloat x;
    GLfloat y;
    GLfloat size;
    GLfloat color[3];
    bool isSelected;
} _Rect;

typedef struct _POINT
{
    float x;
    float y;
} _POINT;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> randomuf(0.0, 1.0);
std::uniform_real_distribution<> randomf(-1.0, 1.0);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

_POINT convertToGLCoord(const _POINT value);

void makeRect();
void seletRect(const _POINT glCoord);
bool checkClickRect(const _POINT glCoord, const int idx);
void moveRect(const _POINT glCoord);
void disableSelectRect();

int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;
GLclampf g_color[4] = {0.0, 0.0, 0.0, 1.0};

bool isMouseDown = false;

_Rect g_rect[5];
_Rect g_temp;
int g_rectCnt = 0;

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Example1");
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < g_rectCnt; ++i)
    {
        glColor3f(g_rect[i].color[0], g_rect[i].color[1], g_rect[i].color[2]);
        glRectf(g_rect[i].x, g_rect[i].y, g_rect[i].x + g_rect[i].size, g_rect[i].y + g_rect[i].size);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
    case 'A':
        makeRect();
        break;
    }
    glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
    _POINT glmouseCoord = convertToGLCoord({(float)x, (float)y});
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        isMouseDown = true;
        seletRect(glmouseCoord);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isMouseDown = false;
        disableSelectRect();
    }
    glutPostRedisplay();
}

GLvoid Motion(int x, int y)
{
    _POINT glMouseCoord = convertToGLCoord({(float)x, (float)y});
    if (isMouseDown)
    {
        moveRect(glMouseCoord);
    }
    glutPostRedisplay();
}

void makeRect()
{
    if (g_rectCnt < 5)
    {
        g_rect[g_rectCnt].x = randomf(gen);
        g_rect[g_rectCnt].y = randomf(gen);
        g_rect[g_rectCnt].size = randomuf(gen);
        g_rect[g_rectCnt].color[0] = randomuf(gen);
        g_rect[g_rectCnt].color[1] = randomuf(gen);
        g_rect[g_rectCnt].color[2] = randomuf(gen);
        g_rect[g_rectCnt].isSelected = false;
        std::cout << "x : " << g_rect[g_rectCnt].x << " y : " << g_rect[g_rectCnt].y << " size : " << g_rect[g_rectCnt].size << std::endl;
        ++g_rectCnt;
    }
    else
    {
        std::cout << "Rect is full" << std::endl;
    }
}

void seletRect(const _POINT glCoord)
{
    for (int i = g_rectCnt - 1; i > -1; --i)
    {
        std::cout << "checking " << i << std::endl;
        if (checkClickRect(glCoord, i))
        {
            g_rect[i].isSelected = true;
            std::cout << "Rect " << i << " is selected" << std::endl;
            g_temp = g_rect[i];
            return;
        }
    }
}

bool checkClickRect(const _POINT glCoord, const int idx)
{
    std::cout << "x : " << glCoord.x << " y : " << glCoord.y << std::endl;
    if (glCoord.x >= g_rect[idx].x && glCoord.y >= g_rect[idx].y)
        if (glCoord.x <= g_rect[idx].x + g_rect[idx].size && glCoord.y <= g_rect[idx].y + g_rect[idx].size)
            return true;
    return false;
}

void disableSelectRect()
{
    for (int i = 0; i < g_rectCnt; ++i)
    {
        g_rect[i].isSelected = false;
    }
}

_POINT convertToGLCoord(const _POINT value)
{
    _POINT glCoord;

    if (value.x <= windowWidth / 2)
        glCoord.x = -(1.0f - value.x / (windowWidth / 2));
    else
        glCoord.x = value.x / (windowWidth / 2) - 1.0f;

    if (value.y <= windowHeight / 2)
        glCoord.y = 1.0f - value.y / (windowHeight / 2);
    else
        glCoord.y = -(value.y / (windowHeight / 2) - 1.0f);

    return glCoord;
}

void moveRect(_POINT glCoord)
{
    for (int i = 0; i < g_rectCnt; ++i)
    {
        if (g_rect[i].isSelected)
        {
            g_rect[i].x = glCoord.x - g_temp.size / 2;
            g_rect[i].y = glCoord.y - g_temp.size / 2;
            return;
        }
    }
}