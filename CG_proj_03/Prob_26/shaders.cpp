#include "shaders.h"

int makeVertexShader(const char *path)
{
    unsigned int ID;

    std::string vertexCode;
    std::ifstream vShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(path);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Vertex shader file not successfully read" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();

    ID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ID, 1, &vShaderCode, NULL);
    glCompileShader(ID);

    int success;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Vertex shader compilation failed: " << infoLog << std::endl;
    }

    return ID;
}

int makeFragmentShader(const char *path)
{
    unsigned int ID;

    std::string fragmentCode;
    std::ifstream fShaderFile;

    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fShaderFile.open(path);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Fragment shader file not successfully read" << std::endl;
    }

    const char *fShaderCode = fragmentCode.c_str();

    ID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ID, 1, &fShaderCode, NULL);
    glCompileShader(ID);

    int success;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Fragment shader compilation failed: " << infoLog << std::endl;
    }

    return ID;
}

GLuint initShader(const char *vertexFile, const char *fragmentFile)
{
    GLuint vShader = makeVertexShader(vertexFile);
    GLuint fShader = makeFragmentShader(fragmentFile);

    GLuint ShaderProgramID;
    ShaderProgramID = glCreateProgram();

    glAttachShader(ShaderProgramID, vShader);
    glAttachShader(ShaderProgramID, fShader);

    glLinkProgram(ShaderProgramID);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

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