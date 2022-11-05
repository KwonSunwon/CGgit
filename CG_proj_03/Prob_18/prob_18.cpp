#include "stdafx.h"
#include "shaders.h"

#define PERSP 0
#define ORTHO 1

#define SOLID 0
#define WIREFRAME 1

#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define UP 3
#define NEAR 4
#define FAR 5

typedef class Planet : public Object
{
private:
    int objectType;

    GLUquadricObj *sphere;

    glm::mat4 model_star;

    glm::vec3 earth_color[3];
    glm::vec3 moon_color[3];

    glm::vec3 earth_pos[3];

    glm::mat4 model_earth[3];
    glm::mat4 model_earth_orbit[3];

    glm::mat4 model_moon[3];
    glm::mat4 model_moon_orbit[3];

    float angle_earth[3];
    float angle_moon[3];

    float radius_earth;
    float radius_moon;

    // use vao
    vector<float> orbit_vertices;

    // draw functions
    void drawOrbit(GLuint shaderProgramID);
    void drawOrbitMoon(int idx, GLuint shaderProgramID);
    void drawStar(GLuint shaderProgramID);
    void drawEarth(int idx, GLuint shaderProgramID);
    void drawMoon(int idx, GLuint shaderProgramID);

public:
    Planet();
    void init() override;
    void initPlanet();
    void initOrbitBuffer(GLuint shaderProgramID);

    void draw_all(GLuint shaderProgramID);

    void update();

    void movement(int direction);
    void objectTypeChange(int type);
    int getObjectType();

    void rotateY(int direction);

} Planet;

Planet planet;

Camera camera;
void initCamera();

void init();

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid keyboard(unsigned char key, int x, int y);

GLclampf g_color[4] = {.5f, .5f, .5f, 1.0f};

// shader variables
GLuint shaderProgramID;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// variables
int projectionType;
bool isRotatePositive = false;
bool isRotateNegative = false;

void updateTimer(int value);

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

    char vertexFile[] = "vertex.vert";
    char fragmentFile[] = "fragment.frag";
    shaderProgramID = initShader(vertexFile, fragmentFile);

    // Initialize
    init();

    planet.init();

    updateTimer(0);

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

    // Camera transformation
    if (projectionType == PERSP)
        projection = camera.getProjection();
    else
        projection = camera.getOrtho();
    view = camera.getView();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Object Draw
    planet.draw_all(shaderProgramID);

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
    // Camera type
    case 'p':
        if (projectionType == PERSP)
            projectionType = ORTHO;
        else
            projectionType = PERSP;
        break;

    // Object type Solid/Wireframe
    case 'm':
        if (planet.getObjectType() == SOLID)
            planet.objectTypeChange(WIREFRAME);
        else
            planet.objectTypeChange(SOLID);
        break;

    // Object Movement
    case 'w':
        planet.movement(UP);
        break;
    case 'a':
        planet.movement(LEFT);
        break;
    case 's':
        planet.movement(DOWN);
        break;
    case 'd':
        planet.movement(RIGHT);
        break;
    case 'z':
        planet.movement(NEAR);
        break;
    case 'x':
        planet.movement(FAR);
        break;

    // Object Rotation
    case 'y': // positive
        if (!isRotatePositive)
        {
            if (isRotateNegative)
                isRotateNegative = false;
            isRotatePositive = true;
        }
        else
            isRotatePositive = false;
        break;
    case 'Y': // negative
        if (!isRotateNegative)
        {
            if (isRotatePositive)
                isRotatePositive = false;
            isRotateNegative = true;
        }
        else
            isRotateNegative = false;
        break;

    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

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
}

Planet::Planet()
{
    objectType = SOLID;

    model_star = glm::mat4(1.0f);
    for (int i = 0; i < 3; i++)
    {
        model_earth[i] = glm::mat4(1.0f);
        model_moon[i] = glm::mat4(1.0f);
    }

    model_earth_orbit[0] = glm::mat4(1.0f);

    model_earth_orbit[1] = glm::mat4(1.0f);
    model_earth_orbit[1] = glm::rotate(model_earth_orbit[1], glm::radians(45.0f), glm::vec3(.0f, .0f, 1.0f));

    model_earth_orbit[2] = glm::mat4(1.0f);
    model_earth_orbit[2] = glm::rotate(model_earth_orbit[2], glm::radians(-45.0f), glm::vec3(.0f, .0f, 1.0f));

    model_moon_orbit[0] = glm::mat4(1.0f);

    model_moon_orbit[1] = glm::mat4(1.0f);
    model_moon_orbit[1] = glm::rotate(model_moon_orbit[1], glm::radians(-45.0f), glm::vec3(.0f, .0f, 1.0f));

    model_moon_orbit[2] = glm::mat4(1.0f);
    model_moon_orbit[2] = glm::rotate(model_moon_orbit[2], glm::radians(45.0f), glm::vec3(.0f, .0f, 1.0f));

    angle_earth[0] = 0.f;
    angle_earth[1] = 90.f;
    angle_earth[2] = 180.f;

    angle_moon[0] = 0.0f;
    angle_moon[1] = 0.0f;
    angle_moon[2] = 0.0f;

    radius_earth = 1.3f;
    radius_moon = .4f;

    earth_color[0] = glm::vec3(0.3f, 0.8f, 1.0f);
    earth_color[1] = glm::vec3(0.2f, 0.6f, 0.2f);
    earth_color[2] = glm::vec3(0.8f, 0.5f, 0.0f);

    moon_color[0] = glm::vec3(0.9f, 0.3f, 0.7f);
    moon_color[1] = glm::vec3(0.1f, 0.2f, 0.3f);
    moon_color[2] = glm::vec3(0.8f, 0.4f, 0.5f);

    rotate = glm::vec3(10.f, 0.f, 0.f);
}

void Planet::init()
{
    initPlanet();
    initOrbitBuffer(shaderProgramID);
}

void Planet::initPlanet()
{
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricTexture(sphere, GL_TRUE);

    // orbit
    float angle = 0;
    float angleStepSize = 1;
    float x, y, z;
    for (angle; angle <= 360; angle += angleStepSize)
    {
        x = radius_earth * cos(glm::radians(angle));
        y = 0.f;
        z = radius_earth * sin(glm::radians(angle));
        orbit_vertices.push_back(x);
        orbit_vertices.push_back(y);
        orbit_vertices.push_back(z);

        // cout << angle << ": " << x << " " << y << " " << z << endl;
    }

    for (angle = 0; angle <= 360; angle += angleStepSize)
    {
        x = radius_moon * cos(glm::radians(angle));
        y = 0.f;
        z = radius_moon * sin(glm::radians(angle));
        orbit_vertices.push_back(x);
        orbit_vertices.push_back(y);
        orbit_vertices.push_back(z);

        // cout << angle << ": " << x << " " << y << " " << z << endl;
    }
}

void Planet::initOrbitBuffer(GLuint shaderProgramID)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, orbit_vertices.size() * sizeof(float), &orbit_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

void Planet::drawOrbit(GLuint shaderProgramID)
{
    glBindVertexArray(vao);

    glUniform3f(glGetUniformLocation(shaderProgramID, "color"), 1.f, 1.f, 1.f);

    glm::mat4 model = transformMat;

    // draw orbit 1 - xz plane
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model * model_earth_orbit[0]));
    glDrawArrays(GL_LINE_LOOP, 0, 360);

    // draw orbit 2 - xz plane (45 degree)
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model * model_earth_orbit[1]));
    glDrawArrays(GL_LINE_LOOP, 0, 360);

    // draw orbit 3 - xz plane (-45 degree)
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model * model_earth_orbit[2]));
    glDrawArrays(GL_LINE_LOOP, 0, 360);
}

void Planet::draw_all(GLuint shaderProgramID)
{
    if (objectType == SOLID)
    {
        gluQuadricDrawStyle(sphere, GLU_FILL);
    }
    else if (objectType == WIREFRAME)
    {
        gluQuadricDrawStyle(sphere, GLU_LINE);
    }

    // Draw Star
    drawStar(shaderProgramID);
    drawOrbit(shaderProgramID);

    for (int i = 0; i < 3; i++)
    {
        drawEarth(i, shaderProgramID);
        drawOrbitMoon(i, shaderProgramID);
        drawMoon(i, shaderProgramID);
    }
}

void Planet::drawStar(GLuint shaderProgramID)
{
    transformMat = glm::mat4(1.0f);
    transformMat = glm::translate(transformMat, pos);
    transformMat = glm::rotate(transformMat, glm::radians(rotate.x), glm::vec3(1.f, 0.f, 0.f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformMat));
    glUniform3f(glGetUniformLocation(shaderProgramID, "color"), .7f, .2f, 0.f);

    gluSphere(sphere, .5f, 60, 60);
}

void Planet::drawEarth(int idx, GLuint shaderProgramID)
{
    model_earth[idx] = glm::mat4(1.0f);
    model_earth[idx] = glm::translate(model_earth[idx], earth_pos[idx]);
    // model_earth[idx] = glm::rotate(model_earth[idx], glm::radians(angle_earth[idx]), glm::vec3(0.f, 1.f, 0.f));
    // model_earth[idx] = glm::translate(model_earth[idx], glm::vec3(radius_earth, 0.f, 0.f));

    glm::mat4 model = transformMat * model_earth_orbit[idx] * model_earth[idx];

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgramID, "color"), earth_color[idx].x, earth_color[idx].y, earth_color[idx].z);

    gluSphere(sphere, .2f, 40, 40);
}

void Planet::drawOrbitMoon(int idx, GLuint shaderProgramID)
{
    model_moon_orbit[idx] = glm::mat4(1.0f);
    model_moon_orbit[idx] = glm::translate(model_moon_orbit[idx], earth_pos[idx]);
    if (idx == 1)
    {
        model_moon_orbit[idx] = glm::rotate(model_moon_orbit[idx], glm::radians(-45.f), glm::vec3(0.f, 0.f, 1.f));
    }
    else if (idx == 2)
    {
        model_moon_orbit[idx] = glm::rotate(model_moon_orbit[idx], glm::radians(45.f), glm::vec3(0.f, 0.f, 1.f));
    }
    glm::mat4 model = transformMat * model_earth_orbit[idx] * model_moon_orbit[idx];

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgramID, "color"), 1.f, 1.f, 1.f);
    glDrawArrays(GL_LINE_LOOP, 361, 360);
}

void Planet::update()
{
    for (int i = 0; i < 3; i++)
    {
        angle_earth[i] += 0.1f + (0.05f * (float)i);

        earth_pos[i].x = radius_earth * cos(glm::radians(angle_earth[i]));
        earth_pos[i].y = 0.f;
        earth_pos[i].z = radius_earth * sin(glm::radians(angle_earth[i]));
    }
    for (int i = 0; i < 3; i++)
    {
        angle_moon[i] += 0.5f - (0.05f * (float)i);
    }
}

void Planet::drawMoon(int idx, GLuint shaderProgramID)
{
    model_moon[idx] = glm::mat4(1.0f);
    model_moon[idx] = glm::translate(model_moon[idx], earth_pos[idx]);
    if (idx == 1)
    {
        model_moon[idx] = glm::rotate(model_moon[idx], glm::radians(-45.f), glm::vec3(0.f, 0.f, 1.f));
    }
    else if (idx == 2)
    {
        model_moon[idx] = glm::rotate(model_moon[idx], glm::radians(45.f), glm::vec3(0.f, 0.f, 1.f));
    }
    model_moon[idx] = glm::rotate(model_moon[idx], glm::radians(angle_moon[idx]), glm::vec3(0.f, 1.f, 0.f));
    model_moon[idx] = glm::translate(model_moon[idx], glm::vec3(radius_moon, 0.f, 0.f));

    glm::mat4 model = transformMat * model_earth_orbit[idx] * model_moon[idx];

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgramID, "color"), moon_color[idx].x, moon_color[idx].y, moon_color[idx].z);

    gluSphere(sphere, .09f, 20, 20);
}

void updateTimer(int value)
{
    planet.update();
    if (isRotatePositive)
        planet.rotateY(1);
    else if (isRotateNegative)
        planet.rotateY(-1);
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, updateTimer, 0);
}

void Planet::movement(int direction)
{
    switch (direction)
    {
        {
        case LEFT:
            pos.x -= 0.1f;
            break;
        case RIGHT:
            pos.x += 0.1f;
            break;
        case UP:
            pos.y += 0.1f;
            break;
        case DOWN:
            pos.y -= 0.1f;
            break;
        case NEAR:
            pos.z -= 0.1f;
            break;
        case FAR:
            pos.z += 0.1f;
            break;
        }
    }
}

void Planet::objectTypeChange(int type)
{
    objectType = type;
}

int Planet::getObjectType()
{
    return objectType;
}

void Planet::rotateY(int direction)
{
    rotate.y += 0.1f * (float)direction;
}