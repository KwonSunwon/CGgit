#include "stdafx.h"

// Shader functions
void makeVertexShaders(char *file);
void makeFragmentShaders(char *file);
GLuint initShader(char *vertexFile, char *fragmentFile);

// void initVAO(GLuint &VAO);
// void initVBO_position(GLuint &VBO_position);
// void initVBO_color(GLuint &VBO_color);
// void initEBO(GLuint &EBO);

char *fileToBuf(char *fileName);