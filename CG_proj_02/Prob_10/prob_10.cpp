#include "stdafx.h"
#include "shaders.h"

// random number generator
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> color(0.f, 1.f);

// gl functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

// gl variables
GLuint windowWidth = 800;
GLuint windowHeight = 800;
GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// shader functions
GLvoid initBuffer();

// shader variables
GLuint shaderID;
GLuint VAO[4];
GLuint VBO_position[4];
GLuint VBO_color[4];
GLuint EBO[3];
mat4 model;

// models original position
// line 0 move
const float lineToTriModel[] = {
    0.0f, -0.1f, 0.0f,
    -0.25f, -0.1f, 0.0f,
    0.25f, -0.1f, 0.0f};
float lineToTriColor[9];

// line 3 move
const float triToRectModel[] = {
    0.0f, 0.4f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.4f, 0.0f, 0.0f,
    0.2f, 0.2f, 0.0f};
const int triToRectIdx[] = {
    0, 1, 2,
    0, 2, 3};
float triToRectColor[12];

// line 1, 3, 5 move
const float rectToPentaModel[] = {
    -0.2f, 0.25f, 0.0f,
    -0.2f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.2f, 0.0f, 0.0f,
    0.2f, 0.25f, 0.0f,
    0.0f, 0.25f, 0.0f};
const int rectToPentaIdx[] = {
    0, 1, 2,
    0, 2, 5,
    5, 2, 4,
    2, 3, 4};
float rectToPentaColor[18];

// line 1, 2, 3, 4, 5 move
const float pentaToDotModel[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.25f, 0.0f,
    -0.25f, 0.0f, 0.0f,
    -0.15f, -0.25f, 0.0f,
    0.15f, -0.25f, 0.0f,
    0.25f, 0.0f, 0.0f};
const int pentaToDotIdx[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 1};
float pentaToDotColor[18];

// models current position
float lineToTri[9];
float triToRect[12];
float rectToPenta[18];
float pentaToDot[18];

float pentaToDotScale;

// functions
void initShape();
void initColor();

void drawShape();
void drawLineToTri();
void drawTriToRect();
void drawRectToPenta();
void drawPentaToDot();

void updateShape();
void updateLineToTri();
void updateTriToRect();
void updateRectToPenta();
void updatePentaToDot();

GLvoid update(int value);

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

    initShape();

    char vertexFile[] = "vertex.vert";
    char fragmentFile[] = "fragment.frag";
    shaderID = initShader(vertexFile, fragmentFile);
    initBuffer();

    glutTimerFunc(50, update, 50);

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderID);
    drawShape();

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
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void initColor()
{
    float r, g, b;
    // line to triangle
    r = color(gen);
    g = color(gen);
    b = color(gen);
    for (int i = 0; i < 9; i += 3)
    {
        lineToTriColor[i] = r;
        lineToTriColor[i + 1] = g;
        lineToTriColor[i + 2] = b;
    }

    // triangle to rectangle
    r = color(gen);
    g = color(gen);
    b = color(gen);
    for (int i = 0; i < 12; i += 3)
    {
        triToRectColor[i] = r;
        triToRectColor[i + 1] = g;
        triToRectColor[i + 2] = b;
    }

    // rectangle to pentagon
    r = color(gen);
    g = color(gen);
    b = color(gen);
    for (int i = 0; i < 18; i += 3)
    {
        rectToPentaColor[i] = r;
        rectToPentaColor[i + 1] = g;
        rectToPentaColor[i + 2] = b;
    }

    // pentagon to dot
    r = color(gen);
    g = color(gen);
    b = color(gen);
    for (int i = 0; i < 18; i += 3)
    {
        pentaToDotColor[i] = r;
        pentaToDotColor[i + 1] = g;
        pentaToDotColor[i + 2] = b;
    }
}

void initShape()
{
    pentaToDotScale = 1.0f;
    copy(lineToTriModel, lineToTriModel + 9, lineToTri);
    copy(triToRectModel, triToRectModel + 12, triToRect);
    copy(rectToPentaModel, rectToPentaModel + 18, rectToPenta);
    copy(pentaToDotModel, pentaToDotModel + 18, pentaToDot);
    initColor();
}

GLvoid initBuffer()
{
    // line to triangle
    glGenVertexArrays(1, &VAO[0]);
    glBindVertexArray(VAO[0]);
    glGenBuffers(1, &VBO_position[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineToTri), lineToTri, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &VBO_color[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineToTriColor), lineToTriColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // triangle to rectangle
    glGenVertexArrays(1, &VAO[1]);
    glBindVertexArray(VAO[1]);
    glGenBuffers(1, &VBO_position[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triToRect), triToRect, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &VBO_color[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triToRectColor), triToRectColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glGenBuffers(1, &EBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triToRectIdx), triToRectIdx, GL_STATIC_DRAW);

    // rectangle to pentagon
    glGenVertexArrays(1, &VAO[2]);
    glBindVertexArray(VAO[2]);
    glGenBuffers(1, &VBO_position[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectToPenta), rectToPenta, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &VBO_color[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectToPentaColor), rectToPentaColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glGenBuffers(1, &EBO[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectToPentaIdx), rectToPentaIdx, GL_STATIC_DRAW);

    // pentagon to dot
    glGenVertexArrays(1, &VAO[3]);
    glBindVertexArray(VAO[3]);
    glGenBuffers(1, &VBO_position[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentaToDot), pentaToDot, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &VBO_color[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentaToDotColor), pentaToDotColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glGenBuffers(1, &EBO[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pentaToDotIdx), pentaToDotIdx, GL_STATIC_DRAW);
}

void drawShape()
{
    drawLineToTri();
    drawTriToRect();
    drawRectToPenta();
    drawPentaToDot();
}

void drawLineToTri()
{
    model = mat4(1.f);

    model = translate(model, vec3(-0.5f, 0.4f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineToTri), lineToTri, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawTriToRect()
{
    model = mat4(1.f);

    model = translate(model, vec3(0.3f, 0.3f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triToRect), triToRect, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawRectToPenta()
{
    model = mat4(1.f);

    model = translate(model, vec3(-0.5f, -0.7f, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectToPenta), rectToPenta, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void drawPentaToDot()
{
    model = mat4(1.f);

    model = translate(model, vec3(0.5f, -0.5f, 0.f));
    model = scale(model, vec3(pentaToDotScale, pentaToDotScale, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentaToDot), pentaToDot, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);
}

GLvoid update(int value)
{
    if (value != 0)
    {
        updateShape();
        glutTimerFunc(50, update, value - 1);
    }
    else
    {
        copy(lineToTriModel, lineToTriModel + 9, lineToTri);
        glutTimerFunc(50, update, 50);
    }
    glutPostRedisplay();
}

void updateShape()
{
    updateLineToTri();
    updateTriToRect();
    updateRectToPenta();
    updatePentaToDot();
}

void updateLineToTri()
{
    lineToTri[1] += 0.007f;
}

void updateTriToRect()
{
    triToRect[9] += 0.003f;
    triToRect[10] += 0.003f;

    if (triToRect[9] >= 0.4f && triToRect[10] >= 0.4f)
    {
        copy(triToRectModel, triToRectModel + 12, triToRect);
    }
}

void updateRectToPenta()
{
    rectToPenta[3] += 0.001f;
    rectToPenta[9] -= 0.001f;
    rectToPenta[16] += 0.0025f;

    if (rectToPenta[16] >= 0.4f)
    {
        copy(rectToPentaModel, rectToPentaModel + 18, rectToPenta);
    }
}

void updatePentaToDot()
{
    pentaToDotScale -= 0.01f;

    if (pentaToDotScale <= 0.0f)
    {
        pentaToDotScale = 1.0f;
    }
}