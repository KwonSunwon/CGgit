#include "stdafx.h"
#include "shaders.h"

// random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> colorUf(0.0, 1.0);

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
    GLfloat color[9];
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
    0.0, 0.35, 0.0,
    0.3, -0.25, 0.0,
    -0.3, -0.25, 0.0};

float vertexColor[9];

GLint windowWidth = 800;
GLint windowHeight = 800;
GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

Triangle triangle[4];
int triangleIdx = -1;
bool scaleUp = true;
float scaleValue = 1.0f;

bool isLine = false;

void initTriangle();
void makeTriangle(Point glCoord);

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

    initTriangle();

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

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 9; ++j)
        {
            vertexColor[j] = triangle[i].color[j];
        }

        shaderID = initShader();
        initBuffer();

        mat4 model = mat4(1.0f);

        model = scale(model, vec3(triangle[i].scale, triangle[i].scale, triangle[i].scale));
        model = translate(model, vec3(triangle[i].position[0], triangle[i].position[1], 0.0f));

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

void initTriangle()
{
    for (int i = 0; i < 4; ++i)
    {
        triangle[i].color[0] = triangle[i].color[3] = triangle[i].color[6] = colorUf(gen);
        triangle[i].color[1] = triangle[i].color[4] = triangle[i].color[7] = colorUf(gen);
        triangle[i].color[2] = triangle[i].color[5] = triangle[i].color[8] = colorUf(gen);
        triangle[i].scale = 1.f;
    }

    triangle[0].position[0] = -0.5f;
    triangle[0].position[1] = 0.5f;

    triangle[1].position[0] = 0.5f;
    triangle[1].position[1] = 0.5f;

    triangle[2].position[0] = -0.5f;
    triangle[2].position[1] = -0.5f;

    triangle[3].position[0] = 0.5f;
    triangle[3].position[1] = -0.5f;
}

void makeTriangle(Point glCoord)
{
    triangleIdx = (triangleIdx + 1) % 4;

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
}