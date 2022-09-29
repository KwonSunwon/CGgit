#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib ")

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <random>
#include <vector>

using namespace std;

typedef struct _Rect
{
    float x;
    float y;
    float width;
    float height;
    float color[3];
} _Rect;

typedef struct _POINT
{
    float x;
    float y;
} _POINT;

// random number generator
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> randomuf(0.0, 1.0);
uniform_real_distribution<> randomf(-1.0, 1.0);

// GLvoid functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

// functions
_POINT convertToGLCoord(const _POINT value);

void makeRect();
bool checkClickRect(const _POINT glCoord);
void moveRect(const _POINT glCoord);
void checkCollision();

// global variables
int windowWidth = 800;
int windowHeight = 600;
GLclampf g_color[4] = {0.0, 0.0, 0.0, 1.0};

bool isMouseDown = false;
bool isEraserSelected = false;
vector<_Rect> g_rect;
_Rect g_eraser;
_Rect g_temp;

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
        std::cout << "GLEW Initialized" << std::endl;

    makeRect();

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

    for (int i = 0; i < g_rect.size(); i++)
    {
        // test
        // cout << i << " : " << g_rect[i].x << ", " << g_rect[i].y << endl;
        glColor3f(g_rect[i].color[0], g_rect[i].color[1], g_rect[i].color[2]);
        glRectf(g_rect[i].x, g_rect[i].y, g_rect[i].x + g_rect[i].width, g_rect[i].y + g_rect[i].height);
    }

    glColor3f(g_eraser.color[0], g_eraser.color[1], g_eraser.color[2]);
    glRectf(g_eraser.x, g_eraser.y, g_eraser.x + g_eraser.width, g_eraser.y + g_eraser.height);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        isMouseDown = true;
        _POINT glCoord = convertToGLCoord({(float)x, (float)y});
        if (checkClickRect(glCoord))
        {
            isEraserSelected = true;
            g_temp = g_eraser;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isMouseDown = false;
        isEraserSelected = false;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // rect init
        g_rect.clear();
        makeRect();
    }
}

GLvoid Motion(int x, int y)
{
    if (isMouseDown)
    {
        // rect move
        _POINT glCoord = convertToGLCoord({(float)x, (float)y});
        if (isEraserSelected)
        {
            moveRect(glCoord);
            checkCollision();
        }
    }
    glutPostRedisplay();
}

_POINT convertToGLCoord(const _POINT value)
{
    _POINT result;
    result.x = (value.x - windowWidth / 2) / (windowWidth / 2);
    result.y = (windowHeight / 2 - value.y) / (windowHeight / 2);
    return result;
}

void makeRect()
{
    while (g_rect.size() < 100)
    {
        _Rect rect;
        rect.x = randomf(gen);
        rect.y = randomf(gen);
        rect.width = 0.05;
        rect.height = 0.05;
        rect.color[0] = randomuf(gen);
        rect.color[1] = randomuf(gen);
        rect.color[2] = randomuf(gen);

        g_rect.push_back(rect);
    }

    g_eraser.x = -0.1;
    g_eraser.y = -0.1;
    g_eraser.width = 0.1;
    g_eraser.height = 0.1;
    g_eraser.color[0] = 1.0;
    g_eraser.color[1] = 1.0;
    g_eraser.color[2] = 1.0;

    g_temp = g_eraser;

    glutPostRedisplay();
}

bool checkClickRect(const _POINT glCoord)
{
    if (glCoord.x >= g_eraser.x && glCoord.x <= g_eraser.x + g_eraser.width &&
        glCoord.y >= g_eraser.y && glCoord.y <= g_eraser.y + g_eraser.height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void moveRect(_POINT glCoord)
{
    g_eraser.x = glCoord.x - g_temp.width / 2;
    g_eraser.y = glCoord.y - g_temp.height / 2;
    return;
}

void checkCollision()
{
    for (int i = 0; i < g_rect.size(); i++)
    {
        if (g_eraser.x + g_eraser.width >= g_rect[i].x && g_eraser.x <= g_rect[i].x + g_rect[i].width &&
            g_eraser.y + g_eraser.height >= g_rect[i].y && g_eraser.y <= g_rect[i].y + g_rect[i].height)
        {
            g_rect.erase(g_rect.begin() + i);
        }
    }
}
