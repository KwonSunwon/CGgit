#include "stdafx.h"
#include "shaders.h"

#define PERSP 0
#define ORTHO 1

#define CLOSE 0
#define OPEN 1

#define UPWARD 0
#define DOWNWARD 1

typedef class
{
private:
    float x, y, z;
    float scale;
    GLuint vao;
    GLuint vbo_pos;
    GLuint vbo_col;
    GLuint ebo;
    glm::mat4 model;
    vector<glm::vec3> vertices;
    vector<glm::vec3> colors;
    vector<GLuint> indices;

public:
    void init();
    void draw(GLuint shaderProgramID);

    void setModel(glm::mat4 model);
    void setVertices(vector<glm::vec3> vertices);
    void setColors(vector<glm::vec3> colors);
    void setIndices(vector<GLuint> indices);
    void setScale(float scale);
    void setX(float x);
    void setY(float y);
    void setZ(float z);

    float getX();
    float getY();
    float getZ();
    float getScale();
    glm::mat4 getModel();
    vector<glm::vec3> getVertices();
    vector<glm::vec3> getColors();
    vector<GLuint> getIndices();
} Object;

typedef class
{
private:
    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;

    // Perspective
    float fovy;
    float aspect;

    // Ortho
    float left;
    float right;
    float bottom;
    float top;

    float zNear;
    float zFar;

public:
    void setView(glm::mat4 view);
    void setProjection(glm::mat4 projection);

    void setEye(glm::vec3 eye);
    void setCenter(glm::vec3 center);
    void setUp(glm::vec3 up);

    void setFovy(float fovy);
    void setAspect(float aspect);
    void setzNear(float zNear);
    void setzFar(float zFar);

    glm::mat4 getView();
    glm::mat4 getProjection();

    void setCamera(GLuint shaderProgramID, int type);
} Camera;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// coordinate object
const GLfloat coord[] = {
    -1.f, 0.0f, 0.0f,
    1.f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.f,
    0.0f, 0.0f, 1.f,
    0.0f, -1.f, 0.0f,
    0.0f, 1.f, 0.0f};
const float coordColor[] = {
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
    0.f, 0.f, 1.f};

// toggle
bool is_DepthTest = false;
int projectionType = PERSP;

bool is_cube_yAxisRotation = false;
bool is_cube_upperFaceRotation = false;
int cube_frontFace = CLOSE;
int cube_sideFace = CLOSE;

int pyramid_pointDirection = UPWARD;
bool is_pyramid_animation = false;

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
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

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw coordinate

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
            glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        }
        else
        {
            projectionType = PERSP;
            gluPerspective(60.0, 1.0, 0.1, 100.0);
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

void Object::init()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo_col);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Object::draw(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}