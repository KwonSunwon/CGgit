#include "stdafx.h"
#include "shaders.h"

typedef class Coord : public Object
{
public:
    void init() override;
    void render(GLuint shaderProgramID) override;
} Coord;
Coord coord;

typedef class Base : public Object
{
public:
    void init() override;
    void render(GLuint shaderProgramID) override;
} Base;
Base base;

typedef class Crane : public Object
{
private:
    float basePosition;
    float middleAngle;
    float armAngle;

    int armDirection;

    glm::mat4 baseModel;
    glm::mat4 middleModel;
    glm::mat4 armRightModel;
    glm::mat4 armLeftModel;

    void drawBase(GLuint shaderProgramID);
    void drawMiddle(GLuint shaderProgramID);
    void drawArmRight(GLuint shaderProgramID);
    void drawArmLeft(GLuint shaderProgramID);

public:
    void init() override;
    void render(GLuint shaderProgramID) override;

    void moveBase(int direction);
    void moveMiddle(int direction);
    void moveArm();
} Crane;
Crane crane;

Camera camera;
void initCamera();
void cameraMoveZ(int direction);
void cameraMoveX(int direction);

GLvoid cameraRotateTimer(int direction);

float getLength(glm::vec3 v, glm::vec3 u);

bool isCameraRotate = false;

void init();

vector<Object *> objects;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

GLvoid moveBaseTimer(int direction);
GLvoid moveMiddleTimer(int direction);
GLvoid moveArmTimer(int value);

bool moveBase = false;
bool moveMiddle = false;
bool moveArm = false;

GLclampf g_color[4] = {0.f, 0.f, 0.f, 1.0f};

// shader variables
GLuint shaderProgramID;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

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

    // Camera
    camera.setCamera(shaderProgramID, 0);

    // Object Draw
    for (int i = 0; i < objects.size(); ++i)
        (*objects[i]).render(shaderProgramID);

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
    // Crane Base Control
    case 'b':
        if (!moveBase)
        {
            moveBase = true;
            moveBaseTimer(1);
        }
        else
            moveBase = false;
        break;
    case 'B':
        if (!moveBase)
        {
            moveBase = true;
            moveBaseTimer(-1);
        }
        else
            moveBase = false;
        break;

    // Crane Middle Control
    case 'm':
        if (!moveMiddle)
        {
            moveMiddle = true;
            moveMiddleTimer(1);
        }
        else
            moveMiddle = false;
        break;
    case 'M':
        if (!moveMiddle)
        {
            moveMiddle = true;
            moveMiddleTimer(-1);
        }
        else
            moveMiddle = false;
        break;

    // Crane Arm Control
    case 't':
        if (!moveArm)
        {
            moveArm = true;
            moveArmTimer(0);
        }
        else
            moveArm = false;
        break;
    case 'T':
        if (!moveArm)
        {
            moveArm = true;
            moveArmTimer(0);
        }
        else
            moveArm = false;
        break;

    // Camera Control
    case 'z':
        cameraMoveZ(1);
        break;
    case 'Z':
        cameraMoveZ(-1);
        break;
    case 'x':
        cameraMoveX(1);
        break;
    case 'X':
        cameraMoveX(-1);
        break;

    case 'y':
        if (!isCameraRotate)
        {
            isCameraRotate = true;
            cameraRotateTimer(1);
        }
        else
            isCameraRotate = false;
        break;
    case 'Y':
        if (!isCameraRotate)
        {
            isCameraRotate = true;
            cameraRotateTimer(-1);
        }
        else
            isCameraRotate = false;
        break;

    case 'r':
        break;
    case 'R':
        break;

    case 'a':
        break;
    case 'A':
        break;

    // All stop
    case 's':
    case 'S':
        moveBase = false;
        moveMiddle = false;
        moveArm = false;
        break;

    // All reset
    case 'c':
        break;
    case 'C':
        break;

    // Exit
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

    camera.setEye(glm::vec3(0.0f, 1.0f, .8f));
}

void init()
{
    initCamera();
    coord.init();
    base.init();
    crane.init();

    objects.push_back(&coord);
    objects.push_back(&base);
    objects.push_back(&crane);
}

void Coord::init()
{
    const vector<float> coordVertices = {
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

    initModel(coordVertices, coordColor);
    initPos();
    pos.y = 0.01f;
    scale = glm::vec3(0.5f, 0.5f, 0.5f);
    initBuffer();
}
void Coord::render(GLuint shaderProgramID)
{
    model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 6);
}

void Base::init()
{
    const vector<float> baseVertices = {
        -1.f, 0.f, -1.f,
        1.f, 0.f, -1.f,
        1.f, 0.f, 1.f,
        -1.f, 0.f, 1.f};
    const vector<float> baseColor = {
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f};
    const vector<GLubyte> baseIndices = {
        0, 1, 2,
        0, 2, 3};

    initModel(baseVertices, baseColor, baseIndices);
    initPos();
    initBuffer();
}
void Base::render(GLuint shaderProgramID)
{
    model = glm::mat4(1.f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

void Crane::init()
{
    const vector<float> craneVertices = {
        // Base
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, .4f, .5f,
        -.5f, .4f, .5f,
        -.5f, .4f, -.5f,
        .5f, .4f, -.5f,

        // Middle
        .3f, 0.f, .3f,
        -.3f, 0.f, .3f,
        -.3f, 0.f, -.3f,
        .3f, 0.f, -.3f,
        .3f, .3f, .3f,
        -.3f, .3f, .3f,
        -.3f, .3f, -.3f,
        .3f, .3f, -.3f,

        // Arm
        .08f, 0.f, .08f,
        -.08f, 0.f, .08f,
        -.08f, 0.f, -.08f,
        .08f, 0.f, -.08f,
        .08f, .5f, .08f,
        -.08f, .5f, .08f,
        -.08f, .5f, -.08f,
        .08f, .5f, -.08f};

    const vector<float> craneColors = {
        // Base
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f,
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f,

        // Middle
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f,
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f,

        // Arm
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f,
        0.f, 1.f, 0.f,
        1.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        1.f, .5f, 1.f};

    const vector<GLubyte> craneIndices = {
        // Base
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        0, 1, 5,
        0, 5, 4,
        3, 2, 6,
        3, 6, 7,
        0, 3, 7,
        0, 7, 4,
        1, 2, 6,
        1, 6, 5,

        // Middle
        8, 9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        8, 9, 13,
        8, 13, 12,
        11, 10, 14,
        11, 14, 15,
        8, 11, 15,
        8, 15, 12,
        9, 10, 14,
        9, 14, 13,

        // Arm
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23,
        16, 17, 21,
        16, 21, 20,
        19, 18, 22,
        19, 22, 23,
        16, 19, 23,
        16, 23, 20,
        17, 18, 22,
        17, 22, 21};

    initModel(craneVertices, craneColors, craneIndices);
    initPos();
    pos.y = 0.01f;
    scale = glm::vec3(0.3f);

    basePosition = 0.f;
    middleAngle = 0.f;
    armAngle = 0.f;
    armDirection = 1;

    initBuffer();
}

void Crane::render(GLuint shaderProgramID)
{
    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);

    drawBase(shaderProgramID);
    drawMiddle(shaderProgramID);
    drawArmRight(shaderProgramID);
    drawArmLeft(shaderProgramID);
}

void Crane::drawBase(GLuint shaderProgramID)
{
    baseModel = glm::mat4(1.f);
    baseModel = glm::translate(baseModel, glm::vec3(basePosition, 0.f, 0.f));
    model = model * baseModel;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
}

void Crane::drawMiddle(GLuint shaderProgramID)
{
    middleModel = glm::mat4(1.f);
    middleModel = glm::translate(middleModel, glm::vec3(0.f, .4f, 0.f));
    middleModel = glm::rotate(middleModel, middleAngle, glm::vec3(0.f, 1.f, 0.f));
    model = model * middleModel;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)36);
}

void Crane::drawArmRight(GLuint shaderProgramID)
{
    armRightModel = glm::mat4(1.f);
    armRightModel = glm::translate(armRightModel, glm::vec3(.15f, .3f, 0.f));
    armRightModel = glm::rotate(armRightModel, glm::radians(armAngle), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model * armRightModel));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)72);
}

void Crane::drawArmLeft(GLuint shaderProgramID)
{
    armLeftModel = glm::mat4(1.f);
    armLeftModel = glm::translate(armLeftModel, glm::vec3(-.15f, .3f, 0.f));
    armLeftModel = glm::rotate(armLeftModel, glm::radians(-armAngle), glm::vec3(1.f, 0.f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model * armLeftModel));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)72);
}

void Crane::moveBase(int direction)
{
    basePosition += (float)direction * 0.01f;
}

void Crane::moveMiddle(int direction)
{
    middleAngle += (float)direction * 0.01f;
}

void Crane::moveArm()
{
    armAngle += .5f * (float)armDirection;
    if (armAngle >= 90.f || armAngle <= -90.f)
    {
        armDirection *= -1;
    }
}

GLvoid moveBaseTimer(int direction)
{
    if (moveBase)
    {
        crane.moveBase(direction);
        glutTimerFunc(1000 / 60, moveBaseTimer, direction);
    }
    glutPostRedisplay();
}

GLvoid moveMiddleTimer(int direction)
{
    if (moveMiddle)
    {
        crane.moveMiddle(direction);
        glutTimerFunc(1000 / 60, moveMiddleTimer, direction);
    }
    glutPostRedisplay();
}

GLvoid moveArmTimer(int value)
{
    if (moveArm)
    {
        crane.moveArm();
        glutTimerFunc(1000 / 60, moveArmTimer, 0);
    }
    glutPostRedisplay();
}

void cameraMoveX(int direction)
{
    glm::vec3 pos = camera.getEye();
    pos.x += (float)direction * 0.1f;
    camera.setEye(pos);

    glm::vec3 center = camera.getCenter();
    center.x += (float)direction * 0.1f;
    camera.setCenter(center);
}

void cameraMoveZ(int direction)
{
    glm::vec3 pos = camera.getEye();
    pos.z += (float)direction * 0.1f;
    camera.setEye(pos);

    glm::vec3 center = camera.getCenter();
    center.z += (float)direction * 0.1f;
    camera.setCenter(center);
}

float getLength(glm::vec3 v, glm::vec3 u)
{
    return sqrt(pow(v.x - u.x, 2) + pow(v.y - u.y, 2) + pow(v.z - u.z, 2));
}

// TODO : 회전이 제대로 되지 않음 
GLvoid cameraRotateTimer(int direction)
{
    if (isCameraRotate)
    {
        glm::vec3 pos = camera.getEye();
        glm::vec3 center = camera.getCenter();

        float length = getLength(pos, center);
        float angle = 0.01f * (float)direction;

        center.x = center.x + length * cos(glm::radians(angle));
        center.z = center.z + length * sin(glm::radians(angle));

        camera.setCenter(center);

        glutTimerFunc(100, cameraRotateTimer, direction);
    }
    glutPostRedisplay();
}