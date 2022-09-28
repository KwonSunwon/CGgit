#include "stdafx.h"
#include "shaders.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

GLuint shaderID;

GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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

    makeVertexShaders();
    makeFragmentShaders();
    shaderID = makeShaderProgram();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}

GLvoid drawScene()
{
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderID);

    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, 1);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}