#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

// ramdom number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> randomuf(0.0, 1.0);
std::uniform_real_distribution<> randomf(-1.0, 1.0);

typedef struct _Rect
{
    GLfloat x;
    GLfloat y;
    GLfloat size;
    GLfloat color[3];
    bool isSelected;
} _Rect;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

GLclampf g_color[4] = {0.0, 0.0, 0.0, 1.0};

float convertToGLCoord(const int value);

bool checkClickRect(const float x, const float y, const int idx);

void makeRect();
void seletRect(const float x, const float y);
void moveRect();
void disableSelectRect();

_Rect g_rect[5];
int g_rectCnt = 0;

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
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
    float glx = convertToGLCoord(x);
    float gly = convertToGLCoord(y);
    std::cout << glx << " " << gly << std::endl;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        std::cout << "Left Button Down" << std::endl;
        seletRect(x, y);
    }
    glutPostRedisplay();
}

GLvoid Motion(int x, int y)
{
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

void seletRect(const float x, const float y)
{
    for (int i = g_rectCnt - 1; i > -1; --i)
    {
        std::cout << "checking " << i << std::endl;
        if (checkClickRect(x, y, i))
        {
            g_rect[i].isSelected = true;
            std::cout << "Rect " << i << " is selected" << std::endl;
            return;
        }
        else
        {
            g_rect[i].isSelected = false;
        }
    }
}

bool checkClickRect(const float x, const float y, const int idx)
{
    std::cout << "checking in click " << idx << std::endl;
    if (x >= g_rect[idx].x && y >= g_rect[idx].y)
        if (x <= g_rect[idx].x + g_rect[idx].size && y <= g_rect[idx].y + g_rect[idx].size)
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

float convertToGLCoord(const int value)
{
    if (value <= 500)
        return 1.0f - (float)value / 500.0f;
    else
        return (float)value / 500.0f - 1.0f;
}