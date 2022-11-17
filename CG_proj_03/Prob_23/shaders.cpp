#include "shaders.h"
GLuint vertexShader;
GLuint fragmentShader;

#pragma region "Shader"

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

#pragma endregion

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
    view = glm::lookAt(eye, center, up);
    projection = glm::perspective(fovy, aspect, zNear, zFar);
    ortho = glm::ortho(left, right, bottom, top, zNear, zFar);

    // test
    // cout << eye.x << " " << eye.y << " " << eye.z << endl;
    // cout << center.x << " " << center.y << " " << center.z << endl;
    // cout << up.x << " " << up.y << " " << up.z << endl;
    // cout << fovy << " " << aspect << " " << zNear << " " << zFar << endl;
    // cout << left << " " << right << " " << bottom << " " << top << endl;
}

glm::vec3 Camera::getEye()
{
    return eye;
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

Object::Object()
{
    initPos();
}

Object::Object(vector<float> vertices, vector<float> colors)
{
    initPos();
    initModel(vertices, colors);
}

Object::Object(vector<float> vertices, vector<float> colors, vector<GLubyte> indices)
{
    initPos();
    initModel(vertices, colors, indices);
}

void Object::initPos()
{
    pos = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Object::setPos(glm::vec3 pos) { this->pos = pos; }
void Object::setPosX(float x) { this->pos.x = x; }
void Object::setPosY(float y) { this->pos.y = y; }
void Object::setPosZ(float z) { this->pos.z = z; }

void Object::setScale(glm::vec3 scale) { this->scale = scale; }
void Object::setScaleX(float x) { this->scale.x = x; }
void Object::setScaleY(float y) { this->scale.y = y; }
void Object::setScaleZ(float z) { this->scale.z = z; }

void Object::setRotate(glm::vec3 rotate) { this->rotate = rotate; }
void Object::setRotateX(float x) { this->rotate.x = x; }
void Object::setRotateY(float y) { this->rotate.y = y; }
void Object::setRotateZ(float z) { this->rotate.z = z; }

glm::vec3 Object::getPos() { return pos; }
glm::vec3 Object::getScale() { return scale; }
glm::vec3 Object::getRotate() { return rotate; }

void Object::setModelPos(vector<float> vertices)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        this->vertices.push_back(vertices[i]);
    }
}
void Object::setModelColor(vector<float> colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        this->colors.push_back(colors[i]);
    }
}
void Object::setModelIndices(vector<GLubyte> indices)
{
    for (int i = 0; i < indices.size(); i++)
    {
        this->indices.push_back(indices[i]);
    }
}

void Object::initModel(vector<float> vertices, vector<float> colors)
{
    setModelPos(vertices);
    setModelColor(colors);
}

void Object::initModel(vector<float> vertices, vector<float> colors, vector<GLubyte> indices)
{
    setModelPos(vertices);
    setModelColor(colors);
    setModelIndices(indices);

    // Test
    // cout << "vertices: " << endl;
    // for (int i = 0; i < this->vertices.size(); i += 3)
    // {
    //     cout << this->vertices[i] << " " << this->vertices[i + 1] << " " << this->vertices[i + 2] << endl;
    // }
    // cout << "colors: " << endl;
    // for (int i = 0; i < this->colors.size(); i += 3)
    // {
    //     cout << this->colors[i] << " " << this->colors[i + 1] << " " << this->colors[i + 2] << endl;
    // }
    // cout << "indices: " << endl;
    // for (int i = 0; i < this->indices.size(); i += 3)
    // {
    //     cout << this->indices[i] << " " << this->indices[i + 1] << " " << this->indices[i + 2] << endl;
    // }
}

void Object::init()
{
    initPos();
    initBuffer();
}

void Object::initBuffer()
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    if (!indices.empty())
    {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
    }
}

void Object::transform(GLuint shaderProgramID)
{
    transformMat = glm::mat4(1.f);
    transformMat = glm::translate(transformMat, pos);
    transformMat = glm::rotate(transformMat, glm::radians(rotate.x), glm::vec3(1.f, 0.f, 0.f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));
    transformMat = glm::rotate(transformMat, glm::radians(rotate.z), glm::vec3(0.f, 0.f, 1.f));
    transformMat = glm::scale(transformMat, scale);

    glUniformMatrix4fv(glGetAttribLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(transformMat));
}

void Object::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, (void *)0);
}

void Object::render(GLuint shaderProgramID)
{
    glUseProgram(shaderProgramID);
    transform(shaderProgramID);
    draw();
}

#pragma endregion