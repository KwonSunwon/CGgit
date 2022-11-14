#include "stdafx.h"
#include "shaders.h"

#include "camera.h"
#include "object.h"

typedef class Robot : public Object
{
private:
    float headHeight;
    float bodyHeight;
    glm::vec3 armRightPos;
    glm::vec3 armLeftPos;
    glm::vec3 legRightPos;
    glm::vec3 legLeftPos;

    float armAngle;
    float legAngle;

    void drawHead(GLuint ID);
    void drawBody(GLuint ID);
    void drawArmRight(GLuint ID);
    void drawArmLeft(GLuint ID);
    void drawLeg(GLuint ID);

public:
    void init() override;
    void render(GLuint shaderProgramID) override;

} Robot;

Robot robot;

Camera camera;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

void testTimer(int value);

GLclampf g_color[4] = {0.f, 0.f, 0.f, 1.0f};

// shader variables
GLuint shaderProgramID;

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
    glEnable(GL_CULL_FACE);

    char vertexFile[] = "vertex.vert";
    char fragmentFile[] = "fragment.frag";
    shaderProgramID = initShader(vertexFile, fragmentFile);

    robot.init();



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

    camera.setCamera(shaderProgramID, 0);
    robot.render(shaderProgramID);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, 800, 800);
}

GLvoid keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {

    // Exit
    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void Robot::init()
{
    const std::vector<float> vertices = {
        // Head
        -1.f, 2.f, -.5f,
        -1.f, 0.f, -.5f,
        1.f, 0.f, -.5f,
        1.f, 2.f, -.5f,

        1.f, 2.f, .5f,
        1.f, 0.f, .5f,
        -1.f, 0.f, .5f,
        -1.f, 2.f, .5f,

        // Head - Nose
        -.2f, .3f, .6f,
        -.2f, 1.f, .6f,
        .2f, 1.f, .6f,
        .2f, .3f, .6f,

        // Body
        -1.5f, 4.f, -1.f,
        -1.5f, 0.f, -1.f,
        1.5f, 0.f, -1.f,
        1.5f, 4.f, -1.f,

        1.5f, 4.f, 1.f,
        1.5f, 0.f, 1.f,
        -1.5f, 0.f, 1.f,
        -1.5f, 4.f, 1.f,

        // Arm Right
        -.5f, 3.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 3.f, -.5f,

        .5f, 3.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 3.f, .5f,

        // Arm Left
        -.5f, 3.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 3.f, -.5f,

        .5f, 3.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 3.f, .5f,

        // Leg
        -.5f, 4.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 4.f, -.5f,

        .5f, 4.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 4.f, .5f};

    const std::vector<GLubyte> indices = {
        // Head
        0, 2, 1, 0, 3, 2, // Front
        4, 6, 5, 4, 7, 6, // Back
        7, 1, 6, 7, 0, 1, // Left
        3, 5, 2, 3, 4, 5, // Right
        7, 3, 0, 7, 4, 3, // Top
        1, 5, 6, 1, 2, 5, // Bottom

        // Nose
        8, 9, 10, 8, 10, 11,

        // Body
        12, 14, 13, 12, 15, 14, // Front
        16, 18, 17, 16, 19, 18, // Back
        19, 13, 18, 19, 12, 13, // Left
        15, 17, 14, 15, 16, 17, // Right
        19, 15, 12, 19, 16, 15, // Top
        13, 17, 18, 13, 14, 17, // Bottom

        // Arm Right
        20, 22, 21, 20, 23, 22, // Front
        24, 26, 25, 24, 27, 26, // Back
        27, 21, 26, 27, 20, 21, // Left
        23, 25, 22, 23, 24, 25, // Right
        27, 23, 20, 27, 24, 23, // Top
        21, 25, 26, 21, 22, 25, // Bottom

        // Arm Left
        28, 30, 29, 28, 31, 30, // Front
        32, 34, 33, 32, 35, 34, // Back
        35, 29, 34, 35, 28, 29, // Left
        31, 33, 30, 31, 32, 33, // Right
        35, 31, 28, 35, 32, 31, // Top
        29, 33, 34, 29, 30, 33, // Bottom

        // Leg
        36, 38, 37, 36, 39, 38, // Front
        40, 42, 41, 40, 43, 42, // Back
        43, 37, 42, 43, 36, 37, // Left
        39, 41, 38, 39, 40, 41, // Right
        43, 39, 36, 43, 40, 39, // Top
        37, 41, 42, 37, 38, 41  // Bottom
    };

    std::vector<float> colors;

    // Head
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(0.f);
        colors.push_back(.4f);
        colors.push_back(.8f);
    }
    // Nose
    for (int i = 0; i < 4; ++i)
    {
        colors.push_back(.5f);
        colors.push_back(.5f);
        colors.push_back(0.f);
    }
    // Body
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(.8f);
        colors.push_back(.4f);
        colors.push_back(0.f);
    }
    // Arm Right
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(0.f);
        colors.push_back(1.f);
        colors.push_back(0.f);
    }
    // Arm Left
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(1.f);
        colors.push_back(0.f);
        colors.push_back(0.f);
    }
    // Leg
    for (int i = 0; i < 8; ++i)
    {
        colors.push_back(0.f);
        colors.push_back(0.f);
        colors.push_back(1.f);
    }

    initModel(vertices, colors, indices);
    initBuffer();

    initPos();
    scale = glm::vec3(.1f, .1f, .1f);

    headHeight = 8.f;
    bodyHeight = 4.f;
    armRightPos = glm::vec3(-1.5f, 4.f, 0.f);
    armLeftPos = glm::vec3(1.5f, 4.f, 0.f);
    legRightPos = glm::vec3(-.5f, 0.f, 0.f);
    legLeftPos = glm::vec3(.5f, 0.f, 0.f);

    rotate.y = 10.f;

    pos = glm::vec3(0.f, -1.f, 0.f);
}

void Robot::render(GLuint shaderProgramID)
{
    glBindVertexArray(vao);

    model = glm::mat4(1.f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, scale);

    drawHead(shaderProgramID);
    drawBody(shaderProgramID);
    drawArmRight(shaderProgramID);
    drawArmLeft(shaderProgramID);
    drawLeg(shaderProgramID);
}

void Robot::drawHead(GLuint ID)
{
    glm::mat4 headModel = glm::mat4(1.f);
    headModel = glm::translate(headModel, glm::vec3(0.f, headHeight, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * headModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);

    // Nose
    glm::mat4 noseModel = glm::mat4(1.f);
    noseModel = glm::translate(noseModel, glm::vec3(0.f, headHeight, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * noseModel));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *)36);
}

void Robot::drawBody(GLuint ID)
{
    glm::mat4 bodyModel = glm::mat4(1.f);
    bodyModel = glm::translate(bodyModel, glm::vec3(0.f, bodyHeight, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * bodyModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)42);
}

void Robot::drawArmLeft(GLuint ID)
{
    glm::mat4 armLeftModel = glm::mat4(1.f);
    armLeftModel = glm::translate(armLeftModel, armLeftPos);
    armLeftModel = glm::rotate(armLeftModel, glm::radians(-armAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * armLeftModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)78);
}

void Robot::drawArmRight(GLuint ID)
{
    glm::mat4 armRightModel = glm::mat4(1.f);
    armRightModel = glm::translate(armRightModel, armRightPos);
    armRightModel = glm::rotate(armRightModel, glm::radians(armAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * armRightModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)114);
}

void Robot::drawLeg(GLuint ID)
{
    glm::mat4 legModel = glm::mat4(1.f);
    legModel = glm::translate(legModel, legRightPos);
    legModel = glm::rotate(legModel, glm::radians(legAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * legModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)150);

    legModel = glm::mat4(1.f);
    legModel = glm::translate(legModel, legLeftPos);
    legModel = glm::rotate(legModel, glm::radians(-legAngle), glm::vec3(1.f, 0.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model * legModel));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)150);
}