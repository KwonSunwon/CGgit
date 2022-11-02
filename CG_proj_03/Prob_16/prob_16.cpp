#include "stdafx.h"
#include "shaders.h"

#define PERSP 0
#define ORTHO 1

#define CLOSE 0
#define OPEN 1

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
    void draw() override;
} Panel;

// const vector<float> panelModel = {
//     -1.f, 0.f, 1.f,
//     -1.f, 0.f, -1.f,
//     1.f, 0.f, -1.f,
//     1.f, 0.f, 1.f};
// const vector<float> panelColor = {
//     1.f, 0.f, 1.f,
//     0.f, 0.f, 1.f,
//     0.f, 1.f, 0.f,
//     1.f, 1.f, 0.f};
// const vector<GLubyte> panelIndices = {
//     0, 1, 2,
//     0, 2, 3};

const vector<float> panelModel = {
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f};
const vector<float> panelColor = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f};
const vector<GLubyte> panelIndices = {
    0, 1, 2, 0, 2, 3, // front
    4, 5, 6, 4, 6, 7, // back
    7, 6, 1, 7, 1, 0, // left
    3, 2, 5, 3, 5, 4, // right
    7, 0, 3, 7, 3, 4, // top
    1, 6, 5, 1, 5, 2, // bottom
};

Panel cube{panelModel, panelColor, panelIndices};

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
float xAngle = -30.f;
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
    cube.render(shaderProgramID);

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
        break;

    case 't': // cube upper face rotation animation
        break;

    case 'f': // cube front face open/close
        break;

    case '1': // cube side face open/close
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
    coord.setRotate(glm::vec3(-30.f, 30.f, 0.f));
    coord.setScale(glm::vec3(1.2f, 1.2f, 1.2f));
    coord.initBuffer();
}

#pragma endregion

void initCube()
{
    cube.setRotate(glm::vec3(-30.f, 30.f, 0.f));
    cube.initBuffer();

    // for (int i = 0; i < 6; ++i)
    // {
    //     cube[i].initModel(panelModel, panelColor, panelIndices);
    //     cube[i].initBuffer();
    // }

    // cube[FRONT].setRotate(glm::vec3(-90.f, 0.f, 0.f));
    // cube[FRONT].setPos(glm::vec3(0.f, 0.f, 1.f));

    // cube[BACK].setRotate(glm::vec3(90.f, 0.f, 0.f));
    // cube[BACK].setPos(glm::vec3(0.f, 0.f, -1.f));

    // cube[LEFT].setRotate(glm::vec3(0.f, 0.f, 90.f));
    // cube[LEFT].setPos(glm::vec3(-1.f, 0.f, 0.f));

    // cube[RIGHT].setRotate(glm::vec3(0.f, 0.f, -90.f));
    // cube[RIGHT].setPos(glm::vec3(1.f, 0.f, 0.f));

    // cube[TOP].setRotate(glm::vec3(0.f, 0.f, 0.f));
    // cube[TOP].setPos(glm::vec3(0.f, 1.f, 0.f));

    // cube[BOTTOM].setRotate(glm::vec3(0.f, 180.f, 0.f));
    // cube[BOTTOM].setPos(glm::vec3(0.f, -1.f, 0.f));
}

void Panel::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Panel::transform(GLuint shaderProgramID)
{
    // cout << "transform" << endl;

    transformMat = glm::mat4(1.f);
    transformMat = glm::rotate(transformMat, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformMat = glm::scale(transformMat, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformMat));
}
