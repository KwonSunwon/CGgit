#include <iostream>
#include <random>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> randomColor(0.0, 1.0);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);

void bgChangeColor();

GLclampf g_bgColor[4] = {
    randomColor(gen),
    randomColor(gen),
    randomColor(gen),
    1.0,
};

void rectInit();
void rectChangeColor();
void rectChangeSize(int);

GLfloat g_rectColor[3];
GLfloat g_rectPos;

bool checkClickRect(const int, const int);

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
        std::cout << "GLEW Initialized" << std::endl;

    rectInit();

    glutMouseFunc(Mouse);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_bgColor[0], g_bgColor[1], g_bgColor[2], g_bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(g_rectColor[0], g_rectColor[1], g_rectColor[2]);
    glRectf(-g_rectPos, -g_rectPos, g_rectPos, g_rectPos);

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
        if (checkClickRect(x, y))
            rectChangeColor();
        else
            bgChangeColor();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if (checkClickRect(x, y))
            rectChangeSize(1);
        else
            rectChangeSize(-1);
    }
    glutPostRedisplay();
}

void rectInit()
{
    g_rectColor[0] = randomColor(gen);
    g_rectColor[1] = randomColor(gen);
    g_rectColor[2] = randomColor(gen);

    g_rectPos = 0.5;
}

void bgChangeColor()
{
    g_bgColor[0] = randomColor(gen);
    g_bgColor[1] = randomColor(gen);
    g_bgColor[2] = randomColor(gen);
}

void rectChangeColor()
{
    g_rectColor[0] = randomColor(gen);
    g_rectColor[1] = randomColor(gen);
    g_rectColor[2] = randomColor(gen);
}

bool checkClickRect(const int x, const int y)
{
    if (x >= (1 - g_rectPos) * 500 && y >= (1 - g_rectPos) * 500)
        if (x <= (1 - g_rectPos) * 500 + g_rectPos * 2 * 500 && y <= (1 - g_rectPos) * 500 + g_rectPos * 2 * 500)
            return true;
    return false;
}

void rectChangeSize(int size)
{
    if ((g_rectPos <= 0.05 && size == -1) || (g_rectPos >= 0.95 && size == 1))
        return;
    g_rectPos += size * 0.05;
}