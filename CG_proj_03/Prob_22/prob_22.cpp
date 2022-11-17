#include "stdafx.h"
#include "shaders.h"

#include "camera.h"
#include "object.h"

typedef class Coord : public Object
{
public:
    void init() override;
    void render(GLuint shaderProgramID) override;
    void initBuffer() override;
} Coord;

typedef class Orbit : public Object
{
public:
    void init() override;
    void render(GLuint shaderProgramID) override;
    void initBuffer() override;
} Orbit;

typedef class Light : public Object
{
private:
    glm::vec3 lightColor;
    glm::vec3 ambientLight;
    int shininess;

    float angle;

public:
    void init() override;
    void render(GLuint shaderProgramID, glm::vec3 viewPos);
    void setLightColor(glm::vec3 lightColor);
    void initBuffer() override;
    void update();
} Light;

typedef class Cube : public Object
{
public:
    void init() override;
    void render(GLuint shaderProgramID) override;
} Cube;

Orbit orbit;
Light light;
Camera camera;
Coord coord;
Cube cube;

void init();
void update(int value);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

GLclampf g_color[4] = {0.2f, 0.2f, 0.2f, 1.0f};

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

    init();
    update(0);

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
    light.render(shaderProgramID, camera.getEye());
    orbit.render(shaderProgramID);
    coord.render(shaderProgramID);
    cube.render(shaderProgramID);

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

void init()
{
    coord.init();
    orbit.init();
    light.init();

    camera.setEye(glm::vec3(2, 1, 5.0f));
    // camera.setYaw(-90.f - 20.f);
    // camera.setPitch(-20.f);
    camera.setzFar(500.f);

    cube.init();
}

void update(int value)
{
    light.update();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, update, 0);
}

#pragma region CoordClass

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
    initBuffer();
    initPos();
    pos.y = 0.01f;
    scale = glm::vec3(100.f, 100.f, 100.f);
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

void Coord::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(2);
}

#pragma endregion

#pragma region CubeClass

void Cube::init()
{
    const vector<float> vert = {
        // cube
        // front
        -1.f, 2.f, -1.f, 0.f, 0.f, -1.f,
        1.f, 0.f, -1.f, 0.f, 0.f, -1.f,
        -1.f, 0.f, -1.f, 0.f, 0.f, -1.f,

        -1.f, 2.f, -1.f, 0.f, 0.f, -1.f,
        1.f, 2.f, -1.f, 0.f, 0.f, -1.f,
        1.f, 0.f, -1.f, 0.f, 0.f, -1.f,

        // back
        1.f, 2.f, 1.f, 0.f, 0.f, 1.f,
        -1.f, 0.f, 1.f, 0.f, 0.f, 1.f,
        1.f, 0.f, 1.f, 0.f, 0.f, 1.f,

        1.f, 2.f, 1.f, 0.f, 0.f, 1.f,
        -1.f, 2.f, 1.f, 0.f, 0.f, 1.f,
        -1.f, 0.f, 1.f, 0.f, 0.f, 1.f,

        // left
        -1.f, 2.f, 1.f, -1.f, 0.f, 0.f,
        -1.f, 0.f, -1.f, -1.f, 0.f, 0.f,
        -1.f, 0.f, 1.f, -1.f, 0.f, 0.f,

        -1.f, 2.f, 1.f, -1.f, 0.f, 0.f,
        -1.f, 2.f, -1.f, -1.f, 0.f, 0.f,
        -1.f, 0.f, -1.f, -1.f, 0.f, 0.f,

        // right
        1.f, 2.f, -1.f, 1.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 1.f, 0.f, 0.f,
        1.f, 0.f, -1.f, 1.f, 0.f, 0.f,

        1.f, 2.f, -1.f, 1.f, 0.f, 0.f,
        1.f, 2.f, 1.f, 1.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 1.f, 0.f, 0.f,

        // top
        -1.f, 2.f, 1.f, 0.f, 1.f, 0.f,
        1.f, 2.f, -1.f, 0.f, 1.f, 0.f,
        -1.f, 2.f, -1.f, 0.f, 1.f, 0.f,

        -1.f, 2.f, 1.f, 0.f, 1.f, 0.f,
        1.f, 2.f, 1.f, 0.f, 1.f, 0.f,
        1.f, 2.f, -1.f, 0.f, 1.f, 0.f,

        // bottom
        -1.f, 0.f, -1.f, 0.f, -1.f, 0.f,
        1.f, 0.f, 1.f, 0.f, -1.f, 0.f,
        -1.f, 0.f, 1.f, 0.f, -1.f, 0.f,

        -1.f, 0.f, -1.f, 0.f, -1.f, 0.f,
        1.f, 0.f, -1.f, 0.f, -1.f, 0.f,
        1.f, 0.f, 1.f, 0.f, -1.f, 0.f};

    vector<float> color;

    for (int i = 0; i < 36; i++)
    {
        color.push_back(0.f);
        color.push_back(1.f);
        color.push_back(0.f);
    }

    initModel(vert, color);
    initBuffer();
    initPos();

    // scale = glm::vec3(.5f, .5f, .5f);
}

void Cube::render(GLuint shaderProgramID)
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

#pragma endregion

#pragma region LightClass

void Light::init()
{
    const vector<float> vert = {
        -.5f, 1.f, -.5f,
        -.5f, 0.f, -.5f,
        .5f, 0.f, -.5f,
        .5f, 1.f, -.5f,

        .5f, 1.f, .5f,
        .5f, 0.f, .5f,
        -.5f, 0.f, .5f,
        -.5f, 1.f, .5f};

    const vector<GLubyte> indices = {
        0, 2, 1, 0, 3, 2, // Front
        4, 6, 5, 4, 7, 6, // Back
        7, 1, 6, 7, 0, 1, // Left
        3, 5, 2, 3, 4, 5, // Right
        7, 3, 0, 7, 4, 3, // Top
        1, 5, 6, 1, 2, 5  // Bottom
    };

    vector<float> color;
    for (int i = 0; i < 8; i++)
    {
        color.push_back(1.f);
        color.push_back(1.f);
        color.push_back(1.f);
    }

    initModel(vert, color, indices);
    initBuffer();
    initPos();

    lightColor = glm::vec3(1.f, 1.f, 1.f);
    pos = glm::vec3(0.5f, 0.5f, 0.f);
    ambientLight = glm::vec3(0.2);
    shininess = 256;
    scale = glm::vec3(.1f, .1f, .1f);

    angle = 0;
}

void Light::render(GLuint shaderProgramID, glm::vec3 viewPos)
{
    glUniform3f(glGetUniformLocation(shaderProgramID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(glGetUniformLocation(shaderProgramID, "lightPos"), pos.x, pos.y, pos.z);
    glUniform3f(glGetUniformLocation(shaderProgramID, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(shaderProgramID, "ambientLight"), ambientLight.x, ambientLight.y, ambientLight.z);
    glUniform1i(glGetUniformLocation(shaderProgramID, "shininess"), shininess);

    model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}

void Light::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(2);

    if (!indices.empty())
    {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
    }
}

void Light::update()
{
    float angleStep = 0.4;
    float radius = 5;

    angle += angleStep;

    if (angle > 360)
        angle = 0;

    pos.x = radius * cos(glm::radians(angle));
    pos.z = radius * sin(glm::radians(angle));
    pos.y = 1.f;

    rotate.y += -angleStep;
}

// light move orbit
void Orbit::init()
{
    float angle = 0;
    float angleStep = 360.f / 100.f;

    for (int i = 0; i < 100; i++)
    {
        float x = cos(glm::radians(angle)) * 2.f;
        float z = sin(glm::radians(angle)) * 2.f;
        float y = 0.5f;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        // cout << x << " " << y << " " << z << endl;

        angle += angleStep;
    }

    for (int i = 0; i < 100; i++)
    {
        colors.push_back(0);
        colors.push_back(0);
        colors.push_back(0);
    }

    initBuffer();
}

void Orbit::render(GLuint shaderProgramID)
{
    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, 100);
    glBindVertexArray(0);
}

void Orbit::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(2);
}

#pragma endregion
