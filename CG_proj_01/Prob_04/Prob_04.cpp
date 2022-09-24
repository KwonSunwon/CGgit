#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <random>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// rect status
#define NONE 0

#define LEFT 10
#define RIGHT 11
#define TOP 12
#define BOTTOM 13

#define LEFT_TOP 100
#define RIGHT_TOP 101
#define LEFT_BOTTOM 102
#define RIGHT_BOTTOM 103

#define VERTICAL 200
#define HORIZONTAL 201

using namespace std;

// random number generator
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> randomuf(0.0, 1.0);
uniform_real_distribution<> randomf(-1.0, 1.0);
uniform_int_distribution<> randomi(0, 4);

typedef struct _Rect
{
    // position and color
    GLfloat x;
    GLfloat y;
    GLfloat x2;
    GLfloat y2;
    GLfloat color[3];

    // status
    int direction;
    int sizeChangeMode;
    bool sizeUp;
} _Rect;

typedef struct _POINT
{
    float x;
    float y;
} _POINT;

// convert to GL coordinate
_POINT convertToGLCoord(const _POINT value);

// call back functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

// timer function
void moveDiagonal(int value);
void moveZigzag(int value);
void changeSize(int value);

// functions
void makeRect(const _POINT glCoord);
int isCollision(const _Rect rect);

// window size
int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;

// global variables
GLclampf g_color[4] = {0.5, 0.5, 0.5, 1.0};

_Rect g_rect[5];
_Rect g_original[5];
int g_rectCnt = 0;

// toggle
bool isMoveDiagonal = false;
bool isMoveZigzag = false;
bool isChangeSize = false;

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

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

// call back functions
GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < g_rectCnt; i++)
    {
        glColor3f(g_rect[i].color[0], g_rect[i].color[1], g_rect[i].color[2]);
        glRectf(g_rect[i].x, g_rect[i].y, g_rect[i].x2, g_rect[i].y2);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // diagonal move and bouncing toggle
    case 'A':
    case 'a':
        if (isMoveDiagonal)
        {
            isMoveDiagonal = false;
            std::cout << "Diagonal move and bouncing is off" << std::endl;
        }
        else
        {
            isMoveDiagonal = true;
            isMoveZigzag = false;
            std::cout << "Diagonal move and bouncing is on" << std::endl;
            glutTimerFunc(50, moveDiagonal, 0);
        }
        break;

    // zigzag move and bouncing toggle
    case 'I':
    case 'i':
        if (isMoveZigzag)
        {
            isMoveZigzag = false;
            std::cout << "Zigzag move and bouncing is off" << std::endl;
        }
        else
        {
            isMoveZigzag = true;
            isMoveDiagonal = false;
            std::cout << "Zigzag move and bouncing is on" << std::endl;
            glutTimerFunc(100, moveZigzag, 0);
        }
        break;

    // size change toggle
    case 'C':
    case 'c':
        if (isChangeSize)
        {
            isChangeSize = false;
            std::cout << "Size change is off" << std::endl;
        }
        else
        {
            isChangeSize = true;
            std::cout << "Size change is on" << std::endl;
            glutTimerFunc(100, changeSize, 0);
        }
        break;

    // all animation stop
    case 'S':
    case 's':
        isMoveDiagonal = false;
        isMoveZigzag = false;
        isChangeSize = false;
        break;

    // move original position
    case 'M':
    case 'm':
        for (int i = 0; i < g_rectCnt; i++)
        {
            g_rect[i].x = g_original[i].x;
            g_rect[i].y = g_original[i].y;
            g_rect[i].x2 = g_original[i].x2;
            g_rect[i].y2 = g_original[i].y2;
        }
        break;

    // remove all objects
    case 'R':
    case 'r':
        g_rectCnt = 0;
        isMoveDiagonal = false;
        isMoveZigzag = false;
        isChangeSize = false;
        break;

    // exit
    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    _POINT glCoord = convertToGLCoord({(float)x, (float)y});
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        makeRect(glCoord);
    }
    glutPostRedisplay();
}

// functions
void makeRect(const _POINT glCoord)
{
    // if rect array is full
    if (g_rectCnt >= 5)
    {
        cout << "Rect array is full" << endl;
        return;
    }

    _Rect rect;
    rect.x = glCoord.x - 0.05;
    rect.y = glCoord.y - 0.05;
    rect.x2 = glCoord.x + 0.05;
    rect.y2 = glCoord.y + 0.05;
    rect.color[0] = randomf(gen);
    rect.color[1] = randomf(gen);
    rect.color[2] = randomf(gen);

    rect.direction = randomi(gen) % 4 + 100;
    rect.sizeChangeMode = randomi(gen) % 2 + 200;
    rect.sizeUp = isChangeSize;

    g_rect[g_rectCnt++] = rect;
    g_original[g_rectCnt - 1] = rect;
}

int isCollision(const _Rect rect)
{
    if (rect.x < -1.0)
        return LEFT;
    else if (rect.x2 > 1.0)
        return RIGHT;
    else if (rect.y < -1.0)
        return BOTTOM;
    else if (rect.y2 > 1.0)
        return TOP;
    else
        return NONE;
}

// convert to GL coordinate
_POINT convertToGLCoord(const _POINT value)
{
    _POINT result;
    result.x = (value.x - windowWidth / 2) / (windowWidth / 2);
    result.y = (windowHeight / 2 - value.y) / (windowHeight / 2);
    return result;
}

// timer function
void moveDiagonal(int value)
{
    if (isMoveDiagonal)
    {
        for (int i = 0; i < g_rectCnt; i++)
        {
            switch (g_rect[i].direction)
            {
            case LEFT_TOP:
                g_rect[i].x -= 0.01;
                g_rect[i].y += 0.01;
                g_rect[i].x2 -= 0.01;
                g_rect[i].y2 += 0.01;
                switch (isCollision(g_rect[i]))
                {
                case LEFT:
                    g_rect[i].direction = RIGHT_TOP;
                    break;
                case TOP:
                    g_rect[i].direction = LEFT_BOTTOM;
                    break;
                }
                break;

            case RIGHT_TOP:
                g_rect[i].x += 0.01;
                g_rect[i].y += 0.01;
                g_rect[i].x2 += 0.01;
                g_rect[i].y2 += 0.01;
                switch (isCollision(g_rect[i]))
                {
                case RIGHT:
                    g_rect[i].direction = LEFT_TOP;
                    break;
                case TOP:
                    g_rect[i].direction = RIGHT_BOTTOM;
                    break;
                }
                break;

            case LEFT_BOTTOM:
                g_rect[i].x -= 0.01;
                g_rect[i].y -= 0.01;
                g_rect[i].x2 -= 0.01;
                g_rect[i].y2 -= 0.01;
                switch (isCollision(g_rect[i]))
                {
                case LEFT:
                    g_rect[i].direction = RIGHT_BOTTOM;
                    break;
                case BOTTOM:
                    g_rect[i].direction = LEFT_TOP;
                    break;
                }
                break;

            case RIGHT_BOTTOM:
                g_rect[i].x += 0.01;
                g_rect[i].y -= 0.01;
                g_rect[i].x2 += 0.01;
                g_rect[i].y2 -= 0.01;
                switch (isCollision(g_rect[i]))
                {
                case RIGHT:
                    g_rect[i].direction = LEFT_BOTTOM;
                    break;
                case BOTTOM:
                    g_rect[i].direction = RIGHT_TOP;
                    break;
                }
                break;
            }
        }
        glutTimerFunc(50, moveDiagonal, 0);
    }
    glutPostRedisplay();
}

void moveZigzag(int value)
{
    if (isMoveZigzag)
    {
        for (int i = 0; i < g_rectCnt; i++)
        {
            switch (g_rect[i].direction)
            {
            case LEFT_TOP:
                g_rect[i].x -= 0.01;
                g_rect[i].y += 0.001;
                g_rect[i].x2 -= 0.01;
                g_rect[i].y2 += 0.001;
                switch (isCollision(g_rect[i]))
                {
                case LEFT:
                    g_rect[i].direction = RIGHT_TOP;
                    break;
                case TOP:
                    g_rect[i].direction = LEFT_BOTTOM;
                    break;
                }
                break;

            case RIGHT_TOP:
                g_rect[i].x += 0.01;
                g_rect[i].y += 0.001;
                g_rect[i].x2 += 0.01;
                g_rect[i].y2 += 0.001;
                switch (isCollision(g_rect[i]))
                {
                case RIGHT:
                    g_rect[i].direction = LEFT_TOP;
                    break;
                case TOP:
                    g_rect[i].direction = RIGHT_BOTTOM;
                    break;
                }
                break;

            case LEFT_BOTTOM:
                g_rect[i].x -= 0.01;
                g_rect[i].y -= 0.001;
                g_rect[i].x2 -= 0.01;
                g_rect[i].y2 -= 0.001;
                switch (isCollision(g_rect[i]))
                {
                case LEFT:
                    g_rect[i].direction = RIGHT_BOTTOM;
                    break;
                case BOTTOM:
                    g_rect[i].direction = LEFT_TOP;
                    break;
                }
                break;

            case RIGHT_BOTTOM:
                g_rect[i].x += 0.01;
                g_rect[i].y -= 0.001;
                g_rect[i].x2 += 0.01;
                g_rect[i].y2 -= 0.001;
                switch (isCollision(g_rect[i]))
                {
                case RIGHT:
                    g_rect[i].direction = LEFT_BOTTOM;
                    break;
                case BOTTOM:
                    g_rect[i].direction = RIGHT_TOP;
                    break;
                }
                break;
            }
        }
        glutTimerFunc(50, moveZigzag, 0);
    }
    glutPostRedisplay();
}

void changeSize(int value)
{
    if (isChangeSize)
    {
        for (int i = 0; i < g_rectCnt; ++i)
        {
            // horizontal size change
            if (g_rect[i].sizeChangeMode == HORIZONTAL)
            {
                if (g_rect[i].sizeUp)
                {
                    g_rect[i].x -= 0.01;
                    g_rect[i].x2 += 0.01;

                    if (abs(g_rect[i].x - g_rect[i].x2) >= 0.4)
                    {
                        g_rect[i].sizeUp = false;
                    }
                }
                else
                {
                    g_rect[i].x += 0.01;
                    g_rect[i].x2 -= 0.01;

                    if (abs(g_rect[i].x - g_rect[i].x2) <= 0.1)
                    {
                        g_rect[i].sizeUp = true;
                    }
                }
            }
            // vertical size change
            else if (g_rect[i].sizeChangeMode == VERTICAL)
            {
                if (g_rect[i].sizeUp)
                {
                    g_rect[i].y -= 0.01;
                    g_rect[i].y2 += 0.01;

                    if (abs(g_rect[i].y - g_rect[i].y2) >= 0.4)
                    {
                        g_rect[i].sizeUp = false;
                    }
                }
                else
                {
                    g_rect[i].y += 0.01;
                    g_rect[i].y2 -= 0.01;

                    if (abs(g_rect[i].y - g_rect[i].y2) <= 0.1)
                    {
                        g_rect[i].sizeUp = true;
                    }
                }
            }
        }
        glutTimerFunc(100, changeSize, 0);
    }
    glutPostRedisplay();
}