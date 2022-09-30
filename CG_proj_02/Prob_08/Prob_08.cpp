#include "stdafx.h"
#include "shaders.h"

#define RECT 0
#define TRI 1

// random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> colorUf(0.f, 1.f);
std::uniform_int_distribution<> randi(0, 3);
std::uniform_real_distribution<> randf(-1.f, 1.f);

// gl window functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

// gl window variables
GLuint windowWidth = 800;
GLuint windowHeight = 800;
GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// gl shader variables
GLuint shaderID[2];

// gl buffer functions
GLvoid initTriangleBuffer();
GLvoid initRectangleBuffer();

// gl buffer variables
GLuint RectangleVAO;
GLuint RectangleVBO_position;
GLuint RectangleEBO_index;
GLuint RectangleVBO_color;

GLuint TriangleVAO;
GLuint TriangleVBO_position;
GLuint TriangleVBO_color;

// rectangle model
const float rectangleModel[] = {
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f};
const int rectangleIndex[] = {
    0, 1, 2,
    2, 3, 0};
const float rectangleColor[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f};
void drawRectangle();

// triangle set
const float triangleModel[] = {
    0.f, 0.15f, 0.f,
    0.1f, -0.15f, 0.f,
    -0.1f, -0.15f, 0.f};
float triangleColor[9];

struct Triangle
{
    GLfloat position[2];
    GLfloat rotate;
    GLfloat color[9];
    int direction;
};

// function
void makeTriangle();
void drawTriangle(Triangle &triangle);

int isCollisionToWall(const Triangle tri);
int isCollisionToRect(const Triangle tri);
void moveZigzag(Triangle &triangle);
void moveDiagonal(Triangle &triangle);
void moveZigzagTimer(int value);
void moveDiagonalTimer(int value);

// variables
Triangle triangle[3];
bool moveInside = true;
bool moveOutside = true;

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

    char vertexFile_rect[] = "vertex_rect.glsl";
    char vertexFile_tri[] = "vertex_tri.glsl";
    char fragmentFile_rect[] = "fragment_rect.glsl";
    char fragmentFile_tri[] = "fragment_tri.glsl";
    shaderID[RECT] = initShader(vertexFile_rect, fragmentFile_rect);
    shaderID[TRI] = initShader(vertexFile_tri, fragmentFile_tri);
    initRectangleBuffer();
    initTriangleBuffer();

    makeTriangle();

    glutTimerFunc(100, moveZigzagTimer, 0);
    glutTimerFunc(100, moveDiagonalTimer, 0);

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    drawRectangle();
    for (int i = 0; i < 3; ++i)
        drawTriangle(triangle[i]);

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
    case 'o':
        // outside triangle stop
        moveOutside = !moveOutside;
        glutTimerFunc(100, moveDiagonalTimer, 0);
        break;
    case 'i':
        // inside triangle stop
        moveInside = !moveInside;
        glutTimerFunc(100, moveZigzagTimer, 0);
        break;
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

GLvoid initRectangleBuffer()
{
    initVAO(RectangleVAO);

    GLint positionLocation = glGetAttribLocation(shaderID[RECT], "vPos");
    initVBO_position(RectangleVBO_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleModel), rectangleModel, GL_STATIC_DRAW);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(positionLocation);

    GLint colorLocation = glGetAttribLocation(shaderID[RECT], "vColor");
    initVBO_color(RectangleVBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleColor), rectangleColor, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(colorLocation);

    initEBO(RectangleEBO_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndex), rectangleIndex, GL_STATIC_DRAW);
}

void drawRectangle()
{
    glUseProgram(shaderID[RECT]);

    mat4 model = mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID[RECT], "model"), 1, GL_FALSE, value_ptr(model));

    glBindVertexArray(RectangleVAO);
    glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
}

GLvoid initTriangleBuffer()
{
    initVAO(TriangleVAO);

    GLint positionLocation = glGetAttribLocation(shaderID[TRI], "vPos");
    initVBO_position(TriangleVBO_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleModel), triangleModel, GL_STATIC_DRAW);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(positionLocation);
}

void drawTriangle(Triangle &triangle)
{
    glUseProgram(shaderID[TRI]);

    mat4 model = mat4(1.0f);
    model = translate(model, vec3(triangle.position[0], triangle.position[1], 0.0f));
    model = rotate(model, radians(triangle.rotate), vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID[TRI], "model"), 1, GL_FALSE, value_ptr(model));

    glUniform3f(glGetUniformLocation(shaderID[TRI], "vColor"), triangle.color[0], triangle.color[1], triangle.color[2]);

    glBindVertexArray(TriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void makeTriangle()
{
    triangle[0].position[0] = 0.0f;
    triangle[0].position[1] = -0.3f;
    triangle[0].rotate = 0;
    triangle[0].direction = 1;

    triangle[1].position[0] = 0.0f;
    triangle[1].position[1] = 0.3f;
    triangle[1].rotate = 180;
    triangle[1].direction = -1;

    triangle[2].position[0] = 0.0f;
    triangle[2].position[1] = -1.0f;
    triangle[2].rotate = 0;
    triangle[2].direction = LEFT_TOP;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 9; ++j)
            triangle[i].color[j] = colorUf(gen);
}

void moveZigzag(Triangle &tri)
{
    tri.position[0] += tri.direction * 0.01f;
    if (tri.position[0] + 0.1f >= 0.5f || tri.position[0] - 0.1f <= -0.5f)
    {
        tri.direction *= -1;
        for (int i = 0; i < 9; ++i)
            tri.color[i] = colorUf(gen);
    }
}

void moveDiagonal(Triangle &tri)
{
    int collision = 0;
    switch (tri.direction)
    {
    case LEFT_TOP:
        tri.position[0] -= 0.01f;
        tri.position[1] += 0.01f;
        if (isCollisionToWall(tri) != -1)
            collision = isCollisionToWall(tri);
        else if (isCollisionToRect(tri) != -1)
            collision = isCollisionToRect(tri);
        switch (collision)
        {
        case LEFT:
            tri.direction = RIGHT_TOP;
            tri.rotate = 270;
            // cout << "left top -> right top" << endl;
            break;
        case TOP:
            tri.direction = LEFT_BOTTOM;
            tri.rotate = 180;
            // cout << "left top -> left bottom" << endl;
            break;
        }
        break;

    case RIGHT_TOP:
        tri.position[0] += 0.01f;
        tri.position[1] += 0.01f;
        if (isCollisionToWall(tri) != -1)
            collision = isCollisionToWall(tri);
        else if (isCollisionToRect(tri) != -1)
            collision = isCollisionToRect(tri);
        switch (collision)
        {
        case RIGHT:
            tri.direction = LEFT_TOP;
            tri.rotate = 90;
            // cout << "right top -> left top" << endl;
            break;
        case TOP:
            tri.direction = RIGHT_BOTTOM;
            tri.rotate = 180;
            // cout << "right top -> right bottom" << endl;
            break;
        }
        break;

    case LEFT_BOTTOM:
        tri.position[0] -= 0.01f;
        tri.position[1] -= 0.01f;
        if (isCollisionToWall(tri) != -1)
            collision = isCollisionToWall(tri);
        else if (isCollisionToRect(tri) != -1)
            collision = isCollisionToRect(tri);
        switch (collision)
        {
        case LEFT:
            tri.direction = RIGHT_BOTTOM;
            tri.rotate = 270;
            // cout << "left bottom -> right bottom" << endl;
            break;
        case BOTTOM:
            tri.direction = LEFT_TOP;
            tri.rotate = 0;
            // cout << "left bottom -> left top" << endl;
            break;
        }
        break;

    case RIGHT_BOTTOM:
        tri.position[0] += 0.01f;
        tri.position[1] -= 0.01f;
        if (isCollisionToWall(tri) != -1)
            collision = isCollisionToWall(tri);
        else if (isCollisionToRect(tri) != -1)
            collision = isCollisionToRect(tri);
        switch (collision)
        {
        case RIGHT:
            tri.direction = LEFT_BOTTOM;
            tri.rotate = 90;
            // cout << "right bottom -> left bottom" << endl;
            break;
        case BOTTOM:
            tri.direction = RIGHT_TOP;
            tri.rotate = 0;
            // cout << "right bottom -> right top" << endl;
            break;
        }
        break;
    }
}

void moveZigzagTimer(int value)
{
    if (moveInside)
    {
        moveZigzag(triangle[0]);
        moveZigzag(triangle[1]);
        glutTimerFunc(30, moveZigzagTimer, 1);
    }
    glutPostRedisplay();
}

void moveDiagonalTimer(int value)
{
    if (moveOutside)
    {
        moveDiagonal(triangle[2]);
        glutTimerFunc(30, moveDiagonalTimer, 1);
    }
    glutPostRedisplay();
}

int isCollisionToWall(const Triangle tri)
{
    if (tri.position[0] - 0.1f < -1.f)
        return LEFT;
    else if (tri.position[0] + 0.1f > 1.f)
        return RIGHT;
    else if (tri.position[1] - 0.15f < -1.f)
        return BOTTOM;
    else if (tri.position[1] + 0.15f > 1.f)
        return TOP;
    else
        return -1;
}

int isCollisionToRect(const Triangle tri)
{
    switch (tri.direction)
    {
    case LEFT_TOP:
        if (tri.position[0] - 0.15f <= 0.5f && tri.position[0] - 0.15f >= -0.5f && tri.position[1] >= -0.5f && tri.position[1] <= 0.5f)
            return LEFT;
        else if (tri.position[0] <= 0.5f && tri.position[0] >= -0.5f && tri.position[1] + 0.15f >= -0.5f && tri.position[1] + 0.15f <= 0.5f)
            return TOP;
        break;

    case RIGHT_TOP:
        if (tri.position[0] + 0.15f <= 0.5f && tri.position[0] + 0.15f >= -0.5f && tri.position[1] >= -0.5f && tri.position[1] <= 0.5f)
            return RIGHT;
        else if (tri.position[0] <= 0.5f && tri.position[0] >= -0.5f && tri.position[1] + 0.15f >= -0.5f && tri.position[1] + 0.15f <= 0.5f)
            return TOP;
        break;

    case LEFT_BOTTOM:
        if (tri.position[0] - 0.15f <= 0.5f && tri.position[0] - 0.15f >= -0.5f && tri.position[1] >= -0.5f && tri.position[1] <= 0.5f)
            return LEFT;
        else if (tri.position[0] <= 0.5f && tri.position[0] >= -0.5f && tri.position[1] - 0.15f >= -0.5f && tri.position[1] - 0.15f <= 0.5f)
            return BOTTOM;
        break;

    case RIGHT_BOTTOM:
        if (tri.position[0] + 0.15f <= 0.5f && tri.position[0] + 0.15f >= -0.5f && tri.position[1] >= -0.5f && tri.position[1] <= 0.5f)
            return RIGHT;
        else if (tri.position[0] <= 0.5f && tri.position[0] >= -0.5f && tri.position[1] - 0.15f >= -0.5f && tri.position[1] - 0.15f <= 0.5f)
            return BOTTOM;
        break;
    }
    return -1;
}