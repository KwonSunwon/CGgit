#include "stdafx.h"
#include "shaders.h"

#define CLICK_CORRECTION 0.05f
#define LEFT_TOP 9
#define LEFT_BOTTOM 0
#define RIGHT_TOP 6
#define RIGHT_BOTTOM 3

// convert window coordinates to OpenGL coordinates
struct Point
{
    float x;
    float y;
};
Point convertToGLCoord(const Point value, const int width, const int height)
{
    Point result;
    result.x = (value.x - width / 2) / (width / 2);
    result.y = (height / 2 - value.y) / (height / 2);
    return result;
}

// gl functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid mouse(int button, int state, int x, int y);
GLvoid motion(int x, int y);

// gl variables
GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLuint windowWidth = 800;
GLuint windowHeight = 600;

// shader variables
GLuint ShaderProgramID;
GLuint VAO;
GLuint VBO_position;

// shader functions
void initBuffer();

// rect
float rect[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};
float tempRect[12];

// motion variables
bool isLeftButtonDown = false;
bool isClickedInside = false;
bool isClickedPoint = false;
Point clickedPoint;
int clickedRectPoint;

// motion functions
void drawRect();
bool checkClickedInside(const Point mousePos);
bool checkClickedPoint(const Point mousePos);
void movePoint(const Point mousePos);
void moveRect(const Point mousePos);

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

    char vertexFile[] = "vertex.glsl";
    char fragmentFile[] = "fragment.glsl";
    ShaderProgramID = initShader(vertexFile, fragmentFile);
    initBuffer();

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    drawRect();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

GLvoid mouse(int button, int state, int x, int y)
{
    Point glCoord = convertToGLCoord({(float)x, (float)y}, windowWidth, windowHeight);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (checkClickedPoint(glCoord))
        {
            isLeftButtonDown = true;
            isClickedPoint = true;
        }
        else if (checkClickedInside(glCoord))
        {
            isLeftButtonDown = true;
            isClickedInside = true;
            copy(begin(rect), end(rect), begin(tempRect));
            clickedPoint = glCoord;
        }
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        isLeftButtonDown = false;
        isClickedInside = false;
        isClickedPoint = false;
    }
    glutPostRedisplay();
}

GLvoid motion(int x, int y)
{
    Point glCoord = convertToGLCoord({(float)x, (float)y}, windowWidth, windowHeight);
    if (isLeftButtonDown && isClickedInside)
    {
        moveRect(glCoord);
    }
    if (isLeftButtonDown && isClickedPoint)
    {
        movePoint(glCoord);
    }
    glutPostRedisplay();
}

void initBuffer()
{
    initVAO(VAO);

    GLint positionLocation = glGetAttribLocation(ShaderProgramID, "vPos");
    initVBO_position(VBO_position);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(positionLocation);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
}

void drawRect()
{
    glUseProgram(ShaderProgramID);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

bool checkClickedInside(const Point mousePos)
{
    if (mousePos.x > rect[0] && mousePos.x < rect[6] &&
        mousePos.y > rect[1] && mousePos.y < rect[7])
        return true;
    return false;
}

void moveRect(const Point mousePos)
{
    for (int i = 0; i < 12; i += 3)
    {
        rect[i] = tempRect[i] - (clickedPoint.x - mousePos.x);
        rect[i + 1] = tempRect[i + 1] - (clickedPoint.y - mousePos.y);
    }
}

bool checkClickedPoint(const Point mousePos)
{
    for (int i = 0; i < 12; i += 3)
    {
        if (mousePos.x > rect[i] - CLICK_CORRECTION && mousePos.x < rect[i] + CLICK_CORRECTION &&
            mousePos.y > rect[i + 1] - CLICK_CORRECTION && mousePos.y < rect[i + 1] + CLICK_CORRECTION)
        {
            clickedRectPoint = i;
            cout << "clicked point : " << clickedRectPoint << endl;
            return true;
        }
    }
    return false;
}

void movePoint(const Point mousePos)
{
    switch (clickedRectPoint)
    {
    case LEFT_TOP:
        rect[9] = mousePos.x;
        rect[10] = mousePos.y;
        break;

    case LEFT_BOTTOM:
        rect[0] = mousePos.x;
        rect[1] = mousePos.y;
        break;

    case RIGHT_TOP:
        rect[6] = mousePos.x;
        rect[7] = mousePos.y;
        break;

    case RIGHT_BOTTOM:
        rect[3] = mousePos.x;
        rect[4] = mousePos.y;
        break;
    }
}
