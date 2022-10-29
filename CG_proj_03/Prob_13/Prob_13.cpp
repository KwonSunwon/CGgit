#include "stdafx.h"
#include "shaders.h"

#define CUBE 1
#define PYRAMID 2

#define SOLID 1
#define WIREFRAME 2

#define POSITIVE 1
#define NEGATIVE -1

// gl functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid specialKeys(int key, int x, int y);

// gl variables
GLclampf g_color[4] = {0.3f, 0.3f, 0.3f, 1.f};
GLuint windowWidth = 800;
GLuint windowHeight = 800;

// shader variables
GLuint ShaderProgramID;
GLuint VAO;
GLuint VBO_position;
GLuint VBO_color;
GLuint EBO;

GLuint VAO_coord;
GLuint VBO_coord_position;
GLuint VBO_coord_color;

GLuint VAO_pyramid;
GLuint VBO_pyramid_position;
GLuint VBO_pyramid_color;
GLuint EBO_pyramid;

glm::mat4 model;

// shader functions
void initBuffer();
void initCubeBuffer();
void initCoordBuffer();
void initPyramidBuffer();

// coordinate
const GLfloat coord[] = {
    -1.f, 0.0f, 0.0f,
    1.f, 0.0f, 0.0f,
    0.0f, -1.f, 0.0f,
    0.0f, 1.f, 0.0f};
const float coordColor[] = {
    0.15f, 0.15f, 0.15f,
    0.15f, 0.15f, 0.15f,
    0.15f, 0.15f, 0.15f,
    0.15f, 0.15f, 0.15f};

// cube model
float cube[] = {
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f};
GLubyte cubeIdx[] = {
    0, 1, 2, 0, 2, 3, // front
    4, 5, 6, 4, 6, 7, // back
    7, 6, 1, 7, 1, 0, // left
    3, 2, 5, 3, 5, 4, // right
    7, 0, 3, 7, 3, 4, // top
    1, 6, 5, 1, 5, 2  // bottom
};
float cubeColor[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f};

// pyramid model
float pyramid[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.0f, 0.5f, 0.0f};
GLubyte pyramidIdx[] = {
    0, 1, 2, // floor
    0, 2, 3, // floor2
    0, 1, 4, // left
    1, 2, 4, // front
    2, 3, 4, // right
    3, 0, 4  // back
};
float pyramidColor[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f};

// functions
void drawCube(int mode);
void drawPyramid(int mode);
void drawCoord();
void xAxisRotationTimer(int value);
void yAxisRotationTimer(int value);
void init();

// variables
int selected = CUBE;
int drawMode = SOLID;
float cx = 0.f;
float cy = 0.f;
float xAxisTheta = -30.f;
float yAxisTheta = 30.f;
bool xAxisRotation = false;
bool yAxisRotation = false;
bool isOnDepthTest = true;

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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

    glEnable(GL_DEPTH_TEST);

    char vertexFile[] = "vertex.vert";
    char fragmentFile[] = "fragment.frag";
    ShaderProgramID = initShader(vertexFile, fragmentFile);
    initBuffer();

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ShaderProgramID);

    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    drawCoord();
    if (selected == CUBE)
    {
        drawCube(drawMode);
    }
    else if (selected == PYRAMID)
    {
        drawPyramid(drawMode);
    }

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
    // change object
    case 'c':
        selected = CUBE;
        break;
    case 'p':
        selected = PYRAMID;
        break;

    case 'h': // on/off depth test
        isOnDepthTest = !isOnDepthTest;
        if (isOnDepthTest)
            glDisable(GL_DEPTH_TEST);
        else
            glEnable(GL_DEPTH_TEST);
        break;

    case 'x': // positive x-axis rotation
        xAxisRotation = !xAxisRotation;
        if (xAxisRotation)
            glutTimerFunc(50, xAxisRotationTimer, POSITIVE);
        break;
    case 'X': // negative x-axis rotation
        xAxisRotation = !xAxisRotation;
        if (xAxisRotation)
            glutTimerFunc(50, xAxisRotationTimer, NEGATIVE);
        break;

    case 'y': // positive y-axis rotation
        yAxisRotation = !yAxisRotation;
        if (yAxisRotation)
            glutTimerFunc(50, yAxisRotationTimer, POSITIVE);
        break;
    case 'Y': // negative y-axis rotation
        yAxisRotation = !yAxisRotation;
        if (yAxisRotation)
            glutTimerFunc(50, yAxisRotationTimer, NEGATIVE);
        break;

    case 'w': // wireframe mode
        drawMode = WIREFRAME;
        break;
    case 'W': // solid mode
        drawMode = SOLID;
        break;

    case 's': // reset
        init();
        break;

    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

GLvoid specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        cx -= 0.05f;
        break;
    case GLUT_KEY_RIGHT:
        cx += 0.05f;
        break;
    case GLUT_KEY_UP:
        cy += 0.05f;
        break;
    case GLUT_KEY_DOWN:
        cy -= 0.05f;
        break;
    }
    glutPostRedisplay();
}

void init()
{
    selected = CUBE;
    drawMode = SOLID;
    cx = 0.f;
    cy = 0.f;
    xAxisTheta = 30.f;
    yAxisTheta = 30.f;
    xAxisRotation = false;
    yAxisRotation = false;
    isOnDepthTest = true;
}

void initBuffer()
{
    initCubeBuffer();
    initPyramidBuffer();
    initCoordBuffer();
}

void initCubeBuffer()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColor), cubeColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);
}

void drawCube(int mode)
{
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(cx, cy, 0.0f));
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    if (mode == SOLID)
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
    else
        for (int i = 0; i < 12; i++)
            glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_BYTE, (void *)(i * 3));
}

void initCoordBuffer()
{
    glGenVertexArrays(1, &VAO_coord);
    glBindVertexArray(VAO_coord);

    glGenBuffers(1, &VBO_coord_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_coord_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_coord_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_coord_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordColor), coordColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}

void drawCoord()
{
    model = glm::mat4(1.f);

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO_coord);
    glDrawArrays(GL_LINES, 0, 4);
}

void initPyramidBuffer()
{
    glGenVertexArrays(1, &VAO_pyramid);
    glBindVertexArray(VAO_pyramid);

    glGenBuffers(1, &VBO_pyramid_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pyramid_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_pyramid_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pyramid_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidColor), pyramidColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO_pyramid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIdx), pyramidIdx, GL_STATIC_DRAW);
}

void drawPyramid(int mode)
{
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(cx, cy, 0.0f));
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO_pyramid);
    if (mode == SOLID)
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (void *)0);
    else
        for (int i = 0; i < 12; i++)
            glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_BYTE, (void *)(i * 3));
}

void xAxisRotationTimer(int direction)
{
    if (xAxisRotation)
    {
        if (direction == POSITIVE)
            xAxisTheta += 1;
        else
            xAxisTheta -= 1;
        glutTimerFunc(50, xAxisRotationTimer, direction);
    }
    glutPostRedisplay();
}

void yAxisRotationTimer(int direction)
{
    if (yAxisRotation)
    {
        if (direction == POSITIVE)
            yAxisTheta += 1;
        else
            yAxisTheta -= 1;
        glutTimerFunc(50, yAxisRotationTimer, direction);
    }
    glutPostRedisplay();
}