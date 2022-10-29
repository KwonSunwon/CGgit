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

// camera class
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