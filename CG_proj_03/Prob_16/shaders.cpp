#include "shaders.h"
GLuint vertexShader;
GLuint fragmentShader;

void makeVertexShaders(char *file)
{
    GLchar *vertexSource;

    // char fileName[] = "vertex.glsl";
    // char *file = fileName;
    vertexSource = fileToBuf(file);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "Error compiling vertex shader: " << errorLog << std::endl;
        return;
    }
}

void makeFragmentShaders(char *file)
{
    GLchar *fragmentSource;

    // char fileName[] = "fragment.glsl";
    // char *file = fileName;
    fragmentSource = fileToBuf(file);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "Error compiling fragment shader: " << errorLog << std::endl;
        return;
    }
}

GLuint initShader(char *vertexFile, char *fragmentFile)
{
    makeVertexShaders(vertexFile);
    makeFragmentShaders(fragmentFile);

    GLuint ShaderProgramID;
    ShaderProgramID = glCreateProgram();

    glAttachShader(ShaderProgramID, vertexShader);
    glAttachShader(ShaderProgramID, fragmentShader);

    glLinkProgram(ShaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
        std::cerr << "Error linking shader program: " << errorLog << std::endl;
        return false;
    }

    return ShaderProgramID;
}

void initVAO(GLuint &VAO)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void initVBO_position(GLuint &VBO_position)
{
    glGenBuffers(1, &VBO_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
}
void initVBO_color(GLuint &VBO_color)
{
    glGenBuffers(1, &VBO_color);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
}

void initEBO(GLuint &EBO)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

char *fileToBuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;
    fptr = fopen(file, "rb"); // Open file for reading
    if (!fptr)                // Return NULL on failure
        return NULL;
    fseek(fptr, 0, SEEK_END);         // Seek to the end of the file
    length = ftell(fptr);             // Find out how many bytes into the file we are
    buf = (char *)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
    fseek(fptr, 0, SEEK_SET);         // Go back to the beginning of the file
    fread(buf, length, 1, fptr);      // Read the contents of the file in to the buffer
    fclose(fptr);                     // Close the file
    buf[length] = 0;                  // Null terminator
    return buf;                       // Return the buffer
}

#pragma region "CameraClass"

// Camera defualt constructor
Camera::Camera()
{
    eye = glm::vec3(0.0f, 0.0f, 3.0f);
    center = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    fovy = 45.0f;
    aspect = 800.0f / 800.0f;

    zNear = 0.1f;
    zFar = 100.0f;

    left = -1.0f;
    right = 1.0f;
    bottom = -1.0f;
    top = 1.0f;
}

void Camera::setCamera()
{
    cout << eye.x << " " << eye.y << " " << eye.z << endl;
    cout << center.x << " " << center.y << " " << center.z << endl;
    cout << up.x << " " << up.y << " " << up.z << endl;
    view = glm::lookAt(eye, center, up);

    cout << fovy << " " << aspect << " " << zNear << " " << zFar << endl;
    projection = glm::perspective(fovy, aspect, zNear, zFar);

    cout << left << " " << right << " " << bottom << " " << top << endl;
    ortho = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::setEye(glm::vec3 eye)
{
    this->eye = eye;
}
void Camera::setCenter(glm::vec3 center)
{
    this->center = center;
}
void Camera::setUp(glm::vec3 up)
{
    this->up = up;
}

void Camera::setFovy(float fovy)
{
    this->fovy = fovy;
}
void Camera::setAspect(float aspect)
{
    this->aspect = aspect;
}
void Camera::setzNear(float zNear)
{
    this->zNear = zNear;
}
void Camera::setzFar(float zFar)
{
    this->zFar = zFar;
}

void Camera::setLeft(float left)
{
    this->left = left;
}
void Camera::setRight(float right)
{
    this->right = right;
}
void Camera::setBottom(float bottom)
{
    this->bottom = bottom;
}
void Camera::setTop(float top)
{
    this->top = top;
}

glm::mat4 Camera::getView()
{
    return view;
}
glm::mat4 Camera::getProjection()
{
    return projection;
}
glm::mat4 Camera::getOrtho()
{
    return ortho;
}
#pragma endregion

#pragma region "ObjectClass"

void Object::initPos()
{
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Object::setPos(glm::vec3 pos)
{
    this->pos = pos;
}
void Object::setScale(glm::vec3 scale)
{
    this->scale = scale;
}
void Object::setRotate(glm::vec3 rotate)
{
    this->rotate = rotate;
}

glm::vec3 Object::getPos()
{
    return pos;
}
glm::vec3 Object::getScale()
{
    return scale;
}
glm::vec3 Object::getRotate()
{
    return rotate;
}

void Object::setModel(const vector<float> vertices, const vector<float> colors, const vector<GLubyte> indices)
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        this->vertices.push_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
    }
    for (int i = 0; i < colors.size(); i += 3)
    {
        this->colors.push_back(glm::vec3(colors[i], colors[i + 1], colors[i + 2]));
    }
    for (int i = 0; i < indices.size(); i++)
    {
        this->indices.push_back(indices[i]);
    }

    // Test
    // cout << "vertices: " << endl;
    // for (int i = 0; i < this->vertices.size(); i++)
    // {
    //     cout << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << endl;
    // }
    // cout << "colors: " << endl;
    // for (int i = 0; i < this->colors.size(); i++)
    // {
    //     cout << this->colors[i].x << " " << this->colors[i].y << " " << this->colors[i].z << endl;
    // }
    // cout << "indices: " << endl;
    // for (int i = 0; i < this->indices.size(); i++)
    // {
    //     cout << this->indices[i] << endl;
    // }
}

void Object::initVAO()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Object::init()
{
    initPos();
    initVAO();
}

void Object::draw()
{
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, (void *)0);
    glBindVertexArray(0);
}

void Object::draw(void *func);

#pragma endregion