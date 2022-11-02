#include "stdafx.h"
#include "shaders.h"

#define PERSP 0
#define ORTHO 1

#define CLOSE 0
#define OPEN 1
#define ANIMATION 2

#define UPWARD 0
#define DOWNWARD 1

// Cube index
#define FRONT 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define TOP 4
#define BOTTOM 5

#pragma region "Coordinate"

typedef class Coord : public Object
{
public:
    Coord(vector<float> vertices, vector<float> colors) : Object(vertices, colors){};

    void transform(GLuint shaderProgramID) override;
    void draw() override;
} Coord;

const vector<float> coordModel = {
    -1.f, 0.0f, 0.0f,
    1.f, 0.0f, 0.0f,

    0.0f, 0.0f, -1.f,
    0.0f, 0.0f, 1.f,

    0.0f, -1.f, 0.0f,
    0.0f, 1.f, 0.0f};
const vector<float> coordColor = {
    0.f, 0.f, 1.f,
    0.f, 0.f, 1.f,
    0.f, 1.f, 0.f,
    0.f, 1.f, 0.f,
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f};
Coord coord{coordModel, coordColor};

void initCoord();

#pragma endregion

#pragma region "Cube"
typedef class Panel : public Object
{
public:
    Panel(vector<float> vertices, vector<float> colors, vector<GLubyte> indices) : Object(vertices, colors, indices){};

    void transform(GLuint shaderProgramID) override;
    void frontTransform(GLuint shaderProgramID);
    void frontRender(GLuint shaderProgramID);
} Panel;

const vector<float> panelModel = {
    -.5f, 0.f, .5f,
    -.5f, 0.f, -.5f,
    .5f, 0.f, -.5f,
    .5f, 0.f, .5f};
const vector<float> panelColor = {
    1.f, 0.f, 1.f,
    0.f, 0.f, 1.f,
    0.f, 1.f, 0.f,
    1.f, 1.f, 0.f};
const vector<GLubyte> panelIndices = {
    0, 1, 2,
    0, 2, 3};

Panel cube[6] = {
    Panel(panelModel, panelColor, panelIndices),
    Panel(panelModel, panelColor, panelIndices),
    Panel(panelModel, panelColor, panelIndices),
    Panel(panelModel, panelColor, panelIndices),
    Panel(panelModel, panelColor, panelIndices),
    Panel(panelModel, panelColor, panelIndices)};

void initCube();

#pragma endregion

Camera camera;
void initCamera();

#pragma region "GLfunctions"
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
#pragma endregion

void init();

GLclampf g_color[4] = {.5f, .5f, .5f, 1.0f};

// shader variables
GLuint shaderProgramID;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// object variables
float xAngle = 30.f;
float yAngle = 30.f;

#pragma region "Toggles"
bool is_DepthTest = true;
int projectionType = PERSP;

bool is_cube_yAxisRotation = false;
bool is_cube_upperFaceRotation = false;
int cube_frontFace = CLOSE;
int cube_sideFace = CLOSE;

int pyramid_pointDirection = UPWARD;
bool is_pyramid_animation = false;
#pragma endregion

#pragma region "Animation"
void cube_yAxisRotation(int value);
void cube_upperFaceRotation(int value);

void cube_frontFaceOpen(int value);
void cube_frontFaceClose(int value);

void cube_sideFaceOpen(int value);
void cube_sideFaceClose(int value);

#pragma endregion

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
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
    shaderProgramID = initShader(vertexFile, fragmentFile);

    cout << "\n";
    cout << "h : Depth Test\n";
    cout << "p : Projection Type\n";
    cout << "\n";
    cout << "y : Cube Y-Axis Rotation\n";
    cout << "t : Cube Upper Face Rotation\n";
    cout << "f : Cube Front Face Open/Close\n";
    cout << "1 : Cube Side Face Open/Close\n";
    cout << "\n";
    cout << "o : Pyramid Animation\n";
    cout << "\n";
    cout << "q : Quit\n";

    // Initialize
    init();

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    // Camera transformation
    if (projectionType == PERSP)
        projection = camera.getProjection();
    else
        projection = camera.getOrtho();

    view = camera.getView();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // draw coordinate
    coord.render(shaderProgramID);

    // draw cube
    cube[FRONT].frontRender(shaderProgramID);
    for (int i = 1; i < 6; i++)
        cube[i].render(shaderProgramID);

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
    case 'h': // depth test
        if (is_DepthTest)
        {
            is_DepthTest = false;
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            is_DepthTest = true;
            glDisable(GL_DEPTH_TEST);
        }
        break;

    case 'p': // ortho/perspective projection
        if (projectionType == PERSP)
        {
            projectionType = ORTHO;
        }
        else
        {
            projectionType = PERSP;
        }
        break;

    case 'y': // y-axis revolution
        if (is_cube_yAxisRotation)
        {
            is_cube_yAxisRotation = false;
        }
        else if (!is_cube_yAxisRotation)
        {
            is_cube_yAxisRotation = true;
            cube_yAxisRotation(0);
        }
        break;

    case 't': // cube upper face rotation animation
        if (is_cube_upperFaceRotation)
        {
            is_cube_upperFaceRotation = false;
        }
        else if (!is_cube_upperFaceRotation)
        {
            is_cube_upperFaceRotation = true;
            cube_upperFaceRotation(0);
        }
        break;

    case 'f': // cube front face open/close
        if (cube_frontFace == CLOSE)
        {
            cube_frontFace = ANIMATION;
            cube_frontFaceOpen(0);
        }
        else if (cube_frontFace == OPEN)
        {
            cube_frontFace = ANIMATION;
            cube_frontFaceClose(0);
        }
        break;

    case '1': // cube side face open/close
        if (cube_sideFace == CLOSE)
        {
            cube_sideFace = ANIMATION;
            cube_sideFaceOpen(0);
        }
        else if (cube_sideFace == OPEN)
        {
            cube_sideFace = ANIMATION;
            cube_sideFaceClose(0);
        }
        break;

    case 'o': // pyramid upward/downward
        break;

    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void initCamera()
{
    camera.setFovy(45.0f);
    camera.setzNear(0.1f);
    camera.setzFar(50.0f);

    camera.setLeft(-2.0f);
    camera.setRight(2.0f);
    camera.setBottom(-2.0f);
    camera.setTop(2.0f);

    camera.setCamera();
}

void init()
{
    initCamera();
    initCoord();
    initCube();
}

#pragma region "Coordination"

void Coord::transform(GLuint shaderProgramID)
{
    transformMat = glm::mat4(1.f);
    transformMat = glm::rotate(transformMat, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMat = glm::scale(transformMat, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformMat));
}

void Coord::draw()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 6);
}

void initCoord()
{
    coord.setRotate(glm::vec3(30.f, 30.f, 0.f));
    coord.setScale(glm::vec3(1.2f, 1.2f, 1.2f));
    coord.initBuffer();
}

#pragma endregion

void initCube()
{
    for (int i = 0; i < 6; ++i)
    {
        cube[i].initBuffer();
    }

    cube[FRONT].setRotate(glm::vec3(90.f, 0.f, 0.f));
    cube[FRONT].setPos(glm::vec3(0.f, 0.0f, 0.5f));

    cube[BACK].setRotate(glm::vec3(-90.f, 0.f, 0.f));
    cube[BACK].setPos(glm::vec3(0.f, 0.0f, -0.5f));

    cube[LEFT].setRotate(glm::vec3(0.f, 0.f, -90.f));
    cube[LEFT].setPos(glm::vec3(-0.5f, 0.0f, 0.f));

    cube[RIGHT].setRotate(glm::vec3(0.f, 0.f, 90.f));
    cube[RIGHT].setPos(glm::vec3(0.5f, 0.0f, 0.f));

    cube[TOP].setRotate(glm::vec3(0.f, 0.f, 0.f));
    cube[TOP].setPos(glm::vec3(0.f, .5f, 0.f));

    cube[BOTTOM].setRotate(glm::vec3(0.f, 180.f, 0.f));
    cube[BOTTOM].setPos(glm::vec3(0.f, -.5f, 0.f));
}

void Panel::transform(GLuint shaderProgramID)
{
    transformMat = glm::mat4(1.f);
    transformMat = glm::rotate(transformMat, glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMat = glm::rotate(transformMat, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMat = glm::translate(transformMat, pos);
    transformMat = glm::rotate(transformMat, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transformMat = glm::scale(transformMat, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformMat));
}

void Panel::frontTransform(GLuint shaderProgramID)
{
    transformMat = glm::mat4(1.f);
    transformMat = glm::rotate(transformMat, glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMat = glm::rotate(transformMat, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMat = glm::translate(transformMat, pos);
    transformMat = glm::translate(transformMat, glm::vec3(0.f, -.5f, 0.f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMat = glm::translate(transformMat, glm::vec3(0.f, 0.f, -0.5f));
    transformMat = glm::scale(transformMat, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformMat));
}
void Panel::frontRender(GLuint shaderProgramID)
{
    frontTransform(shaderProgramID);
    draw();
}

#pragma endregion

void cube_yAxisRotation(int value)
{
    if (is_cube_yAxisRotation)
    {
        yAngle += 0.5f;
        glutTimerFunc(20, cube_yAxisRotation, 0);
    }
    glutPostRedisplay();
}

void cube_upperFaceRotation(int value)
{
    if (is_cube_upperFaceRotation)
    {
        float angle = cube[TOP].getRotate().x;
        angle += 0.5f;
        cube[TOP].setRotateX(angle);
        glutTimerFunc(20, cube_upperFaceRotation, 0);
    }
    glutPostRedisplay();
}

void cube_frontFaceOpen(int value)
{
    if (cube_frontFace == ANIMATION)
    {
        float angle = cube[FRONT].getRotate().x;
        angle += 1.f;
        cube[FRONT].setRotateX(angle);
        if (angle >= 180.f)
        {
            cube_frontFace = OPEN;
        }
        glutTimerFunc(20, cube_frontFaceOpen, 0);
    }
    glutPostRedisplay();
}

void cube_frontFaceClose(int value)
{
    if (cube_frontFace == ANIMATION)
    {
        float angle = cube[FRONT].getRotate().x;
        angle -= 1.f;
        cube[FRONT].setRotateX(angle);
        if (angle <= 90.f)
        {
            cube_frontFace = CLOSE;
        }
        glutTimerFunc(20, cube_frontFaceClose, 0);
    }
    glutPostRedisplay();
}

void cube_sideFaceOpen(int value)
{
    if (cube_sideFace == ANIMATION)
    {
        float pos = cube[LEFT].getPos().y;
        pos += .01f;
        cube[LEFT].setPosY(pos);
        cube[RIGHT].setPosY(pos);
        if (pos >= 1.f)
        {
            cube_sideFace = OPEN;
        }
        glutTimerFunc(20, cube_sideFaceOpen, 0);
    }
    glutPostRedisplay();
}

void cube_sideFaceClose(int value)
{
    if (cube_sideFace == ANIMATION)
    {
        float pos = cube[LEFT].getPos().y;
        pos -= .01f;
        cube[LEFT].setPosY(pos);
        cube[RIGHT].setPosY(pos);
        if (pos <= 0.f)
        {
            cube_sideFace = CLOSE;
        }
        glutTimerFunc(20, cube_sideFaceClose, 0);
    }
    glutPostRedisplay();
}