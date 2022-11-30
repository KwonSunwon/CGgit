#include "stdafx.h"
#include "shaders.h"

#include "camera.h"
#include "object.h"
#include "sphere.h"

typedef class Light : public Object
{
private:
    glm::vec3 lightColor;
    glm::vec3 ambientLight;
    int shininess;

    float radius = 4;
    float angle;
    float angleStep = 0.f;

    bool isOn = true;

    vector<glm::vec3> colorSet;
    unsigned int colorIndex = 0;

public:
    void init() override;
    void render(GLuint shaderProgramID, glm::vec3 viewPos);
    void setLightColor(glm::vec3 lightColor);
    void initBuffer() override;
    void update();

    void setRadius(float radius);
    void setAngleStep(float angleStep);
    void turnOnOff();

    void changeColor();
} Light;

Light light;
Camera camera;

vector<Object *> objects;

int drawObject = 1;
float radius = 4.f;
bool isMoving = false;

void init();
void update(int value);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

GLclampf g_color[4] = {0.2f, 0.2f, 0.2f, 1.0f};

// shader variables
GLuint shaderProgramID;

const vector<glm::vec3> colorSet = {
    glm::vec3(1.0, 0.2, 0.2),
    glm::vec3(0.2, 1.0, 0.2),
    glm::vec3(0.2, 0.2, 1.0),
};

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

    shaderProgramID = initShader("vertex.vert", "fragment.frag");

    for (int i = 0; i < 3; ++i)
    {
        Sphere *sphere = new Sphere();
        sphere->initBuffer();
        sphere->colorInit(colorSet[i]);
        sphere->setPos(glm::vec3(-3 * i, 0, -0.5 * i));
        objects.push_back(sphere);
    }

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

    for (int i = 0; i < objects.size(); ++i)
        objects[i]->render(shaderProgramID);

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
    case 'c':
        light.changeColor();
        break;

    case 'r':
        if (isMoving)
            light.setAngleStep(0.f);
        else
            light.setAngleStep(0.4f);
        isMoving = !isMoving;
        break;

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
    light.init();

    camera.setEye(glm::vec3(2, 1, 10.0f));
    camera.setzFar(500.f);
}

void update(int value)
{
    light.update();

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, update, 0);
}

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
    pos = glm::vec3(0.0f, 0.f, 1.f);
    ambientLight = glm::vec3(0.2);
    shininess = 256;
    scale = glm::vec3(.1f, .1f, .1f);

    angle = 80;

    colorSet.push_back(glm::vec3(1.f, 1.f, 1.f));
    colorSet.push_back(glm::vec3(1.f, 0.5f, 0.5f));
    colorSet.push_back(glm::vec3(0.5f, 1.f, 0.5f));
    colorSet.push_back(glm::vec3(0.5f, 0.5f, 1.f));
}

void Light::render(GLuint shaderProgramID, glm::vec3 viewPos)
{
    if (isOn)
        glUniform1i(glGetUniformLocation(shaderProgramID, "isOn"), 1);
    else
        glUniform1i(glGetUniformLocation(shaderProgramID, "isOn"), 0);

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
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), &_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(2);

    if (!_indices.empty())
    {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size(), &_indices[0], GL_STATIC_DRAW);
    }
}

void Light::update()
{
    angle += angleStep;

    if (angle > 360)
        angle = 0;

    pos.x = radius * cos(glm::radians(angle));
    pos.z = radius * sin(glm::radians(angle));
    pos.y = .5f;

    rotate.y += -angleStep;
}

void Light::setRadius(float value)
{
    radius = value;
}

void Light::setAngleStep(float value)
{
    angleStep = value;
}

void Light::turnOnOff()
{
    isOn = !isOn;
    cout << "Light is " << (isOn ? "on" : "off") << endl;
}

void Light::changeColor()
{
    colorIndex++;
    if (colorIndex >= colorSet.size())
        colorIndex = 0;

    lightColor = colorSet[colorIndex];
}

#pragma endregion
