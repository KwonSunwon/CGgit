#include "stdafx.h"
#include "shaders.h"

#define PERSP 0
#define ORTHO 1

#define CLOSE 0
#define OPEN 1

#define UPWARD 0
#define DOWNWARD 1

/*
// typedef class
// {
// private:
//     float x, y, z;
//     float scale;
//     GLuint vao;
//     GLuint vbo_pos;
//     GLuint vbo_col;
//     GLuint ebo;
//     glm::mat4 model;
//     vector<glm::vec3> vertices;
//     vector<glm::vec3> colors;
//     vector<GLuint> indices;

// public:
//     void init();
//     void draw(GLuint shaderProgramID);

//     void setModel(glm::mat4 model);
//     void setVertices(vector<glm::vec3> vertices);
//     void setColors(vector<glm::vec3> colors);
//     void setIndices(vector<GLuint> indices);
//     void setScale(float scale);
//     void setX(float x);
//     void setY(float y);
//     void setZ(float z);

//     float getX();
//     float getY();
//     float getZ();
//     float getScale();
//     glm::mat4 getModel();
//     vector<glm::vec3> getVertices();
//     vector<glm::vec3> getColors();
//     vector<GLuint> getIndices();
// } Object;
*/


Camera camera;
Object myCube;

void initCamera();

void init();

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

GLclampf g_color[4] = {.5f, .5f, .5f, 1.0f};


const vector<float> cube = {
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f};

const vector<GLubyte> cubeIdx = {
    0, 1, 2, 0, 2, 3, // front
    4, 5, 6, 4, 6, 7, // back
    7, 6, 1, 7, 1, 0, // left
    3, 2, 5, 3, 5, 4, // right
    7, 0, 3, 7, 3, 4, // top
    1, 6, 5, 1, 5, 2  // bottom
};

const vector<float> cubeColor = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f};

void drawCube();
void initCubeBuffer();

GLuint VAO;
GLuint VBO_position;
GLuint VBO_color;
GLuint EBO;

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

GLuint VAO_coord;
GLuint VBO_coord_position;
GLuint VBO_coord_color;

void initCoordBuffer();
void drawCoord();

// shader variables
GLuint shaderProgramID;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// object variables
float xAngle = 30.f;
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

    // char vertexFile[] = "vertex.vert";
    // char fragmentFile[] = "fragment.frag";
    // shaderProgramID = initShader(vertexFile, fragmentFile);

    // init();

    myCube.setModel(cube, cubeColor, cubeIdx);

    // test
    // initCubeBuffer();

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

    if (projectionType == PERSP)
        projection = camera.getProjection();
    else
        projection = camera.getOrtho();

    view = camera.getView();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // draw coordinate
    drawCube();
    drawCoord();

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

/*
// void Object::init()
// {
//     glGenVertexArrays(1, &vao);
//     glBindVertexArray(vao);

//     glGenBuffers(1, &vbo_pos);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//     glEnableVertexAttribArray(0);

//     glGenBuffers(1, &vbo_col);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
//     glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//     glEnableVertexAttribArray(1);

//     glGenBuffers(1, &ebo);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

//     glBindVertexArray(0);
// }

// void Object::draw(GLuint shaderProgramID)
// {
//     glUseProgram(shaderProgramID);

//     glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

//     glBindVertexArray(vao);
//     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//     glBindVertexArray(0);
// }
*/

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
    initCoordBuffer();
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
    glUseProgram(shaderProgramID);

    model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO_coord);
    glDrawArrays(GL_LINES, 0, 6);
}

void drawCube()
{
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f, 0.f, 0.0f));
    model = glm::rotate(model, glm::radians(-30.f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(30.f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void *)0);
}

// void initCubeBuffer()
// {
//     glGenVertexArrays(1, &VAO);
//     glBindVertexArray(VAO);

//     glGenBuffers(1, &VBO_position);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//     glEnableVertexAttribArray(0);

//     glGenBuffers(1, &VBO_color);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColor), cubeColor, GL_STATIC_DRAW);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//     glEnableVertexAttribArray(1);

//     glGenBuffers(1, &EBO);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);
// }
