#include "shaders.h"
#include "stdafx.h"

#define SET1 1
#define SET2 2

#define CCW -1.f
#define CW 1.f

#define R_CLOSE_CENTER 0b00000001
#define R_AWAY_CENTER 0b00000010
#define L_CLOSE_CENTER 0b00000100
#define L_AWAY_CENTER 0b00001000

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
GLuint gluObjShaderID;

GLuint VAO;
GLuint VBO_position;
GLuint VBO_color;
GLuint EBO;

GLuint VAO_coord;
GLuint VBO_coord_position;
GLuint VBO_coord_color;

glm::mat4 model;

// shader functions
void initBuffer();
void initCubeBuffer();
void initCoordBuffer();

// coordinate
const GLfloat coord[] = {-1.f, 0.0f, 0.0f, 1.f, 0.0f, 0.0f, 0.0f, 0.0f, -1.f,
                         0.0f, 0.0f, 1.f, 0.0f, -1.f, 0.0f, 0.0f, 1.f, 0.0f};
const float coordColor[] = {1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f,
                            0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f};

// cube model
float cube[] = {-0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
                0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
GLubyte cubeIdx[] = {
    0, 1, 2, 0, 2, 3, // front
    4, 5, 6, 4, 6, 7, // back
    7, 6, 1, 7, 1, 0, // left
    3, 2, 5, 3, 5, 4, // right
    7, 0, 3, 7, 3, 4, // top
    1, 6, 5, 1, 5, 2, // bottom
};
float cubeColor[] = {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                     0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};

// functions
void drawCoord();
// SET 1
void drawCube();
void drawSphere();
// SET 2
void drawCylinder();
void drawCone();

void xAxisRotationRightTimer(int value);
void yAxisRotationRightTimer(int value);
void xAxisRotationLeftTimer(int value);
void yAxisRotationLeftTimer(int value);
void yAxisRevolutionTimer(int value);

void init();

// animations
void makeSpiral();
void spiralAnimationTimer(int value);
void patrolAnimationTimer(int value);
void changeAnimationTimer(int value);

// animation variables
bool isSpiralAnimation = false;
bool isPatrolAnimation = false;
bool isChangeAnimation = false;

// variables
int selected = SET1;

float rx = 0.5f;
float ry = 0.2f;
float rz = 0.0f;
float rSize = 0.3f;
float rSizeFromCenter = 1.0f;

float lx = -0.5f;
float ly = 0.2f;
float lz = 0.0f;
float lSize = 0.3f;
float lSizeFromCenter = 1.0f;

float xAxisThetaRight = 0.f;
float yAxisThetaRight = 0.f;
bool xAxisRotationRight = false;
bool yAxisRotationRight = false;

float xAxisThetaLeft = 0.f;
float yAxisThetaLeft = 0.f;
bool xAxisRotationLeft = false;
bool yAxisRotationLeft = false;

float xAxisTheta = -30.f;
float yAxisTheta = 30.f;
bool yAxisRevolution = false;

// glu obj
GLUquadricObj *sphere = gluNewQuadric();
GLUquadricObj *cylinder = gluNewQuadric();
GLUquadricObj *cone = gluNewQuadric();

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
    char gluObjVertexFile[] = "glu_obj.vert";
    char gluObjFragmentFile[] = "glu_obj.frag";
    gluObjShaderID = initShader(gluObjVertexFile, gluObjFragmentFile);

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

    drawCoord();
    if (selected == SET1)
    {
        drawCube();
        drawSphere();
    }
    else
    {
        drawCylinder();
        drawCone();
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) { glViewport(0, 0, w, h); }

GLvoid keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // change object
    case 'o':
        if (selected == SET1)
            selected = SET2;
        else
            selected = SET1;
        break;

    case 'z': // CW x-axis rotation for right object
        xAxisRotationRight = !xAxisRotationRight;
        if (xAxisRotationRight)
            glutTimerFunc(50, xAxisRotationRightTimer, CW);
        break;
    case 'Z': // CCW x-axis rotation for right object
        xAxisRotationRight = !xAxisRotationRight;
        if (xAxisRotationRight)
            glutTimerFunc(50, xAxisRotationRightTimer, CCW);
        break;

    case 'x': // CW y-axis rotation for right object
        yAxisRotationRight = !yAxisRotationRight;
        if (yAxisRotationRight)
            glutTimerFunc(50, yAxisRotationRightTimer, CW);
        break;
    case 'X': // CCW y-axis rotation for right object
        yAxisRotationRight = !yAxisRotationRight;
        if (yAxisRotationRight)
            glutTimerFunc(50, yAxisRotationRightTimer, CCW);
        break;

    case 'n': // CW x-axis rotation for left object
        xAxisRotationLeft = !xAxisRotationLeft;
        if (xAxisRotationLeft)
            glutTimerFunc(50, xAxisRotationLeftTimer, CW);
        break;
    case 'N': // CCW x-axis rotation for left object
        xAxisRotationLeft = !xAxisRotationLeft;
        if (xAxisRotationLeft)
            glutTimerFunc(50, xAxisRotationLeftTimer, CCW);
        break;

    case 'm': // CW y-axis rotation for left object
        yAxisRotationLeft = !yAxisRotationLeft;
        if (yAxisRotationLeft)
            glutTimerFunc(50, yAxisRotationLeftTimer, CW);
        break;
    case 'M': // CCW y-axis rotation for left object
        yAxisRotationLeft = !yAxisRotationLeft;
        if (yAxisRotationLeft)
            glutTimerFunc(50, yAxisRotationLeftTimer, CCW);
        break;

    case 'r': // CW y-axis revolution
        yAxisRevolution = !yAxisRevolution;
        if (yAxisRevolution)
            glutTimerFunc(50, yAxisRevolutionTimer, CW);
        break;
    case 'R': // CCW y-axis revolution
        yAxisRevolution = !yAxisRevolution;
        if (yAxisRevolution)
            glutTimerFunc(50, yAxisRevolutionTimer, CCW);
        break;

    case '1': // make spiral and move object following spiral
        break;
    case '2': // move object to center and far away from center
        isPatrolAnimation = !isPatrolAnimation;
        if (isPatrolAnimation)
            glutTimerFunc(50, patrolAnimationTimer, 0);
        break;
    case '3': // both object move to each other's position
        break;

    // l-object
    // move
    case 'w': // positive z-axis
        lz += 0.01f;
        break;
    case 's': // negative z-axis
        lz -= 0.01f;
        break;
    case 'a': // negative x-axis
        lx -= 0.01f;
        break;
    case 'd': // positive x-axis
        lx += 0.01f;
        break;
    case 'q': // positive y-axis
        ly += 0.01f;
        break;
    case 'e': // negative y-axis
        ly -= 0.01f;
        break;

    // scaling
    case 'f':
        lSize += 0.01f;
        break;
    case 'g':
        lSize -= 0.01f;
        break;

    // scaling from center
    case 'F':
        lSizeFromCenter += 0.01f;
        break;
    case 'G':
        lSizeFromCenter -= 0.01f;
        break;

    // r-object
    // scaling
    case '.':
        rSize += 0.01f;
        break;
    case '/':
        rSize -= 0.01f;
        break;

    // scaling from center
    case '>':
        rSizeFromCenter += 0.01f;
        break;
    case '?':
        rSizeFromCenter -= 0.01f;
        break;

    // both
    // move
    case 'i': // negative z-axis
        rz += 0.01f;
        lz += 0.01f;
        break;
    case 'k': // positive z-axis
        rz -= 0.01f;
        lz -= 0.01f;
        break;
    case 'j': // negative x-axis
        rx -= 0.01f;
        lx -= 0.01f;
        break;
    case 'l': // positive x-axis
        rx += 0.01f;
        lx += 0.01f;
        break;

        // case 's': // reset
        //     init();
        //     break;

    case 0x1B: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

GLvoid specialKeys(int key, int x, int y)
{
    switch (key)
    {
    // r-object move
    case GLUT_KEY_LEFT: // negative x-axis
        rx -= 0.05f;
        break;
    case GLUT_KEY_RIGHT: // positive x-axis
        rx += 0.05f;
        break;
    case GLUT_KEY_UP: // positive z-axis
        rz += 0.05f;
        break;
    case GLUT_KEY_DOWN: // negative z-axis
        rz -= 0.05f;
        break;
    case GLUT_KEY_PAGE_UP: // positive y-axis
        ry += 0.05f;
        break;
    case GLUT_KEY_PAGE_DOWN: // negative y-axis
        ry -= 0.05f;
        break;
    }
    glutPostRedisplay();
}

void init()
{
    selected = SET1;
    xAxisThetaRight = 0.f;
    yAxisThetaRight = 0.f;
    xAxisRotationRight = false;
    yAxisRotationRight = false;
    xAxisThetaLeft = 0.f;
    yAxisThetaLeft = 0.f;
    xAxisRotationLeft = false;
    yAxisRotationLeft = false;
    xAxisTheta = -30.f;
    yAxisTheta = 30.f;
    yAxisRevolution = false;
}

void initBuffer()
{
    initCubeBuffer();
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

void drawCube()
{
    glUseProgram(ShaderProgramID);

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(rSizeFromCenter, rSizeFromCenter,
                                        rSizeFromCenter)); // scaling from center
    model = glm::translate(model, glm::vec3(rx, ry, rz));
    model = glm::rotate(model, glm::radians(xAxisThetaRight), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisThetaRight), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(rSize, rSize, rSize));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
    for (int i = 0; i < 12; i++)
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_BYTE, (void *)(i * 3));
}

void drawSphere()
{
    glUseProgram(gluObjShaderID);

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(lSizeFromCenter, lSizeFromCenter,
                                        lSizeFromCenter)); // scaling from center
    model = glm::translate(model, glm::vec3(lx, ly, lz));
    model = glm::rotate(model, glm::radians(xAxisThetaLeft), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisThetaLeft), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(lSize, lSize, lSize));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    gluQuadricDrawStyle(sphere, GLU_LINE);
    gluSphere(sphere, 1.0, 20, 20);
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
    glUseProgram(ShaderProgramID);

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    glBindVertexArray(VAO_coord);
    glDrawArrays(GL_LINES, 0, 6);
}

void drawCylinder()
{
    glUseProgram(gluObjShaderID);

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(rSizeFromCenter, rSizeFromCenter,
                                        rSizeFromCenter)); // scaling from center
    model = glm::translate(model, glm::vec3(rx, ry, rz));
    model = glm::rotate(model, glm::radians(xAxisThetaRight), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisThetaRight), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(rSize, rSize, rSize));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    gluQuadricDrawStyle(cylinder, GLU_LINE);
    gluCylinder(cylinder, 1.0, 1.0, 2.0, 20, 8);
}

void drawCone()
{
    glUseProgram(gluObjShaderID);

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(xAxisTheta), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(yAxisTheta), glm::vec3(0.f, 1.f, 0.f));
    model = glm::scale(model, glm::vec3(lSizeFromCenter, lSizeFromCenter,
                                        lSizeFromCenter)); // scaling from center
    model = glm::translate(model, glm::vec3(lx, ly, lz));
    model = glm::rotate(model, glm::radians(xAxisThetaLeft), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAxisThetaLeft), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(lSize, lSize, lSize));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    gluQuadricDrawStyle(cone, GLU_LINE);
    gluCylinder(cone, 1.0, 0.0, 2.0, 20, 8);
}

void xAxisRotationRightTimer(int value)
{
    if (xAxisRotationRight)
    {
        xAxisThetaRight += value;
        glutTimerFunc(50, xAxisRotationRightTimer, value);
    }
    glutPostRedisplay();
}

void yAxisRotationRightTimer(int value)
{
    if (yAxisRotationRight)
    {
        yAxisThetaRight += value;
        glutTimerFunc(50, yAxisRotationRightTimer, value);
    }
    glutPostRedisplay();
}

void xAxisRotationLeftTimer(int value)
{
    if (xAxisRotationLeft)
    {
        xAxisThetaLeft += value;
        glutTimerFunc(50, xAxisRotationLeftTimer, value);
    }
    glutPostRedisplay();
}

void yAxisRotationLeftTimer(int value)
{
    if (yAxisRotationLeft)
    {
        yAxisThetaLeft += value;
        glutTimerFunc(50, yAxisRotationLeftTimer, value);
    }
    glutPostRedisplay();
}

void yAxisRevolutionTimer(int value)
{
    if (yAxisRevolution)
    {
        yAxisTheta += value;
        glutTimerFunc(50, yAxisRevolutionTimer, value);
    }
    glutPostRedisplay();
}

void patrolAnimationTimer(int value)
{
    if (isPatrolAnimation)
    {
        if (abs(rx) > 0.f)
        {
            rx -= rx / 50.f;
        }
        if (abs(lx) > 0.f)
        {
            lx -= lx / 50.f;
        }
        glutTimerFunc(50, patrolAnimationTimer, value);
    }
    glutPostRedisplay();
}