#include "stdafx.h"
#include "shaders.h"

#define SIZE 0.006f
#define MAX_PARTICLES 440
#define SPIRAL_SIZE 0.25f

// random number generator
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0.f, 1.f);
uniform_int_distribution<> dis_int(0, 1);

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

// gl functions
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid mouse(int button, int state, int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);

// gl variables
GLuint windowWidth = 800;
GLuint windowHeight = 800;
GLclampf g_color[4] = {dis(gen), dis(gen), dis(gen), 1.0f};

// shader functions
GLvoid initBuffer();

// shader variables
GLuint shaderID;
GLuint VAO;
GLuint VBO;
GLuint EBO;

// model
const float rectModel[] = {
    -SIZE, SIZE, 0.0f,
    SIZE, SIZE, 0.0f,
    SIZE, -SIZE, 0.0f,
    -SIZE, -SIZE, 0.0f};
const GLuint rectIdx[] = {
    0, 1, 2,
    0, 2, 3};

// variables
struct Rect
{
    Point pos;
    float rotate;
};

vector<Rect> rects;
bool clockwise;
bool drawing = false;
bool addRect;
Point start;
float radius;
float theta;

// functions
void draw();
void drawingTimer(int value);
void makeSpiral();

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

    char vertexFile[] = "vertex.glsl";
    char fragmentFile[] = "fragment.glsl";
    shaderID = initShader(vertexFile, fragmentFile);
    initBuffer();

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

    draw();

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

GLvoid mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !drawing)
    {
        g_color[0] = dis(gen);
        g_color[1] = dis(gen);
        g_color[2] = dis(gen);

        start = convertToGLCoord({(float)x, (float)y}, windowWidth, windowHeight);
        drawing = true;
        addRect = true;
        radius = SPIRAL_SIZE;
        theta = 0;
        clockwise = dis_int(gen);

        glutTimerFunc(100, drawingTimer, 0);
        glutPostRedisplay();
    }
}

GLvoid keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        exit(0);
        break;
    }
}

GLvoid initBuffer()
{
    initVAO(VAO);

    initVBO_position(VBO);
    GLint posAttrib = glGetAttribLocation(shaderID, "vPos");
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectModel), rectModel, GL_STATIC_DRAW);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    initEBO(EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIdx), rectIdx, GL_STATIC_DRAW);
}

void drawingTimer(int value)
{
    if (drawing)
    {
        makeSpiral();
        glutTimerFunc(10, drawingTimer, 0);
    }
    glutPostRedisplay();
}

void draw()
{
    if (rects.empty())
        return;

    glUseProgram(shaderID);
    glBindVertexArray(VAO);

    for (int i = 0; i < rects.size(); i++)
    {
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(rects[i].pos.x, rects[i].pos.y, 0.0f));
        model = rotate(model, rects[i].rotate, vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void makeSpiral()
{
    if (drawing)
    {
        if (addRect)
        {
            Rect rect;
            if (clockwise)
            {
                // make left spiral
                rect.rotate = theta;
                rect.pos.x = (start.x - SPIRAL_SIZE) + cos(rect.rotate) * radius;
                rect.pos.y = start.y + sin(rect.rotate) * radius;
                rects.push_back(rect);

                // make right spiral
                rect.rotate = theta - M_PI;
                rect.pos.x = (start.x + SPIRAL_SIZE) + cos(rect.rotate) * radius;
                rect.pos.y = start.y + sin(rect.rotate) * radius;
                rects.push_back(rect);

                theta -= 0.1f;
                radius -= 0.001f;
            }
            else
            {
                // make left spiral
                rect.rotate = theta;
                rect.pos.x = (start.x - SPIRAL_SIZE) + cos(rect.rotate) * radius;
                rect.pos.y = start.y + sin(rect.rotate) * radius;
                rects.push_back(rect);

                // make right spiral
                rect.rotate = theta - M_PI;
                rect.pos.x = (start.x + SPIRAL_SIZE) + cos(rect.rotate) * radius;
                rect.pos.y = start.y + sin(rect.rotate) * radius;
                rects.push_back(rect);

                theta += 0.1f;
                radius -= 0.001f;
            }
            if (rects.size() == MAX_PARTICLES)
            {
                addRect = false;
            }
        }
        else
        {
            if (rects.empty())
            {
                drawing = false;
                return;
            }
            rects.pop_back();
            rects.pop_back();
        }
    }
}