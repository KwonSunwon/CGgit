#include "stdafx.h"
#include "shaders.h"

#define CUBE 1
#define TETRAHEDRON 2

// gl functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

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

GLuint VAO_tetrahedron;
GLuint VBO_tetrahedron_position;
GLuint VBO_tetrahedron_color;
GLuint EBO_tetrahedron;

glm::mat4 model;

// shader functions
void initBuffer();
void initCubeBuffer();
void initCoordBuffer();
void initTetrahedronBuffer();

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
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f};
GLubyte cubeIdx[] = {
    4, 5, 6, 4, 6, 7, // front
    0, 1, 2, 0, 2, 3, // back
    7, 6, 1, 7, 1, 0, // left
    3, 2, 5, 3, 5, 4, // right
    0, 3, 4, 0, 4, 7, // top
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

// tetrahedron
float tetrahedron[] = {
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.0f, -0.5f, -0.5f,
    0.0f, 0.5f, 0.0f};
GLubyte tetrahedronIdx[] = {
    0, 1, 2, // floor
    0, 1, 3, // left
    1, 2, 3, // right
    0, 2, 3  // back
};
float tetrahedronColor[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f};

// functions
void drawCube(int count, int indices);
void drawCoord();
void drawTetrahedron(int count, int indices, bool mode);

// variables
int drawCount;
int indices;
int selected;
bool floorDraw;

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

    floorDraw = false;
    selected = CUBE;
    drawCount = 36;
    indices = 0;

    char vertexFile[] = "vertex.vert";
    char fragmentFile[] = "fragment.frag";
    ShaderProgramID = initShader(vertexFile, fragmentFile);
    initBuffer();

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ShaderProgramID);

    drawCoord();
    if (selected == CUBE)
        drawCube(drawCount, indices);
    else if (selected == TETRAHEDRON)
        drawTetrahedron(drawCount, indices, floorDraw);

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
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case 'a':
    case 'b':
    case 'c':
        selected = CUBE;
        break;
    case '7':
    case '8':
    case '9':
    case '0':
    case 'e':
    case 'f':
    case 'g':
        selected = TETRAHEDRON;
        break;
    }
    switch (key)
    {
    // cube
    case '1': // front
        drawCount = 6;
        indices = 0;
        break;
    case '2': // back
        drawCount = 6;
        indices = 6;
        break;
    case '3': // left
        drawCount = 6;
        indices = 12;
        break;
    case '4': // right
        drawCount = 6;
        indices = 18;
        break;
    case '5': // top
        drawCount = 6;
        indices = 24;
        break;
    case '6': // bottom
        drawCount = 6;
        indices = 30;
        break;

    case 'a': // front + back
        drawCount = 12;
        indices = 0;
        break;
    case 'b': // left + right
        drawCount = 12;
        indices = 12;
        break;
    case 'c': // top + bottom
        drawCount = 12;
        indices = 24;
        break;

    // tetrahedron
    case '7': // floor
        drawCount = 3;
        indices = 0;
        floorDraw = false;
        break;
    case '8':
        drawCount = 3;
        indices = 3;
        floorDraw = false;
        break;
    case '9':
        drawCount = 3;
        indices = 6;
        floorDraw = false;
        break;
    case '0':
        drawCount = 3;
        indices = 9;
        floorDraw = false;
        break;

    case 'e':
        drawCount = 3;
        indices = 3;
        floorDraw = true;
        break;
    case 'f':
        drawCount = 3;
        indices = 6;
        floorDraw = true;
        break;
    case 'g':
        drawCount = 3;
        indices = 9;
        floorDraw = true;
        break;

    case 'r':
        if (selected == CUBE)
            drawCount = 36;
        else if (selected == TETRAHEDRON)
        {
            drawCount = 12;
            floorDraw = false;
        }
        indices = 0;
        break;

    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void initBuffer()
{
    initCubeBuffer();
    initCoordBuffer();
    initTetrahedronBuffer();
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

void drawCube(int count, int indices)
{
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-10.f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, (void *)indices);
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

void initTetrahedronBuffer()
{
    glGenVertexArrays(1, &VAO_tetrahedron);
    glBindVertexArray(VAO_tetrahedron);

    glGenBuffers(1, &VBO_tetrahedron_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tetrahedron_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron), tetrahedron, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_tetrahedron_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tetrahedron_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronColor), tetrahedronColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO_tetrahedron);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_tetrahedron);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedronIdx), tetrahedronIdx, GL_STATIC_DRAW);
}

void drawTetrahedron(int count, int indices, bool mode)
{
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-10.f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO_tetrahedron);
    if (mode)
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (void *)0);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, (void *)indices);
}