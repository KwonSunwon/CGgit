#include "stdafx.h"
#include "shaders.h"

// random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> colorUf(0.0, 1.0);
std::uniform_real_distribution<> randi(0, 3);

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

struct Triangle
{
    GLfloat position[2];
    GLfloat scale;
    GLfloat rotate;
    GLfloat color[9];
    int direction;
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid mouse(int button, int state, int x, int y);

GLvoid initBuffer();

GLuint shaderID;

GLuint VBO_position;
GLuint VBO_color;
GLuint VAO;

const float vertexPosition[] = {
    0.0, 0.4, 0.0,
    0.3, -0.2f, 0.0,
    -0.3, -0.2f, 0.0};

float vertexColor[9];

GLint windowWidth = 800;
GLint windowHeight = 800;
GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

Triangle triangle[4];
int triangleIdx = 0;
int triangleCnt = 0;
bool scaleUp = true;
float scaleValue = 1.0f;
bool isMove = true;

bool isLine = false;

void makeTriangle(Point glCoord);
void moveTriangle();
int isCollisionToWall(const Triangle tri);
void moveTimer(int value);

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

    shaderID = initShader();

    glutTimerFunc(50, moveTimer, 1);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < triangleCnt; i++)
    {
        for (int j = 0; j < 9; ++j)
        {
            vertexColor[j] = triangle[i].color[j];
        }

        initBuffer();

        mat4 model = mat4(1.0f);

        model = translate(model, vec3(triangle[i].position[0], triangle[i].position[1], 0.0f));
        model = rotate(model, radians(triangle[i].rotate), vec3(0.0f, 0.0f, 1.0f));
        model = scale(model, vec3(triangle[i].scale, triangle[i].scale, triangle[i].scale));

        GLuint modelLoc = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

        if (isLine)
            glDrawArrays(GL_LINE_LOOP, 0, 3);
        else
            glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid initBuffer()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);

    GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(pAttribute);

    GLint cAttribute = glGetAttribLocation(shaderID, "vColor");
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(cAttribute);
}

GLvoid keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        isLine = !isLine;
        break;
    case 's':
        isMove = !isMove;
        glutTimerFunc(50, moveTimer, 1);
        break;
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
        // make triangle
        makeTriangle(glCoord);
    }
    glutPostRedisplay();
}

void makeTriangle(Point glCoord)
{
    triangle[triangleIdx].position[0] = glCoord.x;
    triangle[triangleIdx].position[1] = glCoord.y;

    if (scaleUp)
    {
        scaleValue += 0.1f;
        if (scaleValue >= 2.0f)
            scaleUp = false;
    }
    else
    {
        scaleValue -= 0.1f;
        if (scaleValue <= 1.0f)
            scaleUp = true;
    }
    triangle[triangleIdx].scale = scaleValue;

    triangle[triangleIdx].color[0] = triangle[triangleIdx].color[3] = triangle[triangleIdx].color[6] = colorUf(gen);
    triangle[triangleIdx].color[1] = triangle[triangleIdx].color[4] = triangle[triangleIdx].color[7] = colorUf(gen);
    triangle[triangleIdx].color[2] = triangle[triangleIdx].color[5] = triangle[triangleIdx].color[8] = colorUf(gen);

    triangle[triangleIdx].direction = randi(gen);
    triangle[triangleIdx].rotate = 0;

    if (triangleCnt < 4)
        triangleCnt++;
    triangleIdx = (triangleIdx + 1) % 4;
}

void moveTriangle()
{
    for (int i = 0; i < triangleCnt; i++)
    {
        switch (triangle[i].direction)
        {
        case LEFT_TOP:
            triangle[i].position[0] -= 0.01f;
            triangle[i].position[1] += 0.01f;
            switch (isCollisionToWall(triangle[i]))
            {
            case LEFT:
                triangle[i].direction = RIGHT_TOP;
                triangle[i].rotate = 270;
                break;
            case TOP:
                triangle[i].direction = LEFT_BOTTOM;
                triangle[i].rotate = 180;
                break;
            }
            break;

        case RIGHT_TOP:
            triangle[i].position[0] += 0.01f;
            triangle[i].position[1] += 0.01f;
            switch (isCollisionToWall(triangle[i]))
            {
            case RIGHT:
                triangle[i].direction = LEFT_TOP;
                triangle[i].rotate = 90;
                break;
            case TOP:
                triangle[i].direction = RIGHT_BOTTOM;
                triangle[i].rotate = 180;
                break;
            }
            break;

        case LEFT_BOTTOM:
            triangle[i].position[0] -= 0.01f;
            triangle[i].position[1] -= 0.01f;
            switch (isCollisionToWall(triangle[i]))
            {
            case LEFT:
                triangle[i].direction = RIGHT_BOTTOM;
                triangle[i].rotate = 270;
                break;
            case BOTTOM:
                triangle[i].direction = LEFT_TOP;
                triangle[i].rotate = 0;
                break;
            }
            break;

        case RIGHT_BOTTOM:
            triangle[i].position[0] += 0.01f;
            triangle[i].position[1] -= 0.01f;
            switch (isCollisionToWall(triangle[i]))
            {
            case RIGHT:
                triangle[i].direction = LEFT_BOTTOM;
                triangle[i].rotate = 90;
                break;
            case BOTTOM:
                triangle[i].direction = RIGHT_TOP;
                triangle[i].rotate = 0;
                break;
            }
            break;
        }
    }
}

int isCollisionToWall(const Triangle tri)
{
    if (tri.position[0] < -1.f)
        return LEFT;
    else if (tri.position[0] > 1.f)
        return RIGHT;
    else if (tri.position[1] < -1.f)
        return BOTTOM;
    else if (tri.position[1] > 1.f)
        return TOP;
    else
        return -1;
}

void moveTimer(int value)
{
    if (isMove)
    {
        moveTriangle();
        glutTimerFunc(20, moveTimer, 1);
    }
    glutPostRedisplay();
}