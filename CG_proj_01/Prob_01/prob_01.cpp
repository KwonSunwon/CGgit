#include <iostream>
#include <random>

#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

GLclampf g_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
bool g_colorChagneTimer = false;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> randomColor(0.0, 1.0);

void colorChangeTimer(int);

void main(int argc, char **argv) //--- 윈도우 출력하고 콜백함수 설정
{
    //--- 윈도우 생성하기
    glutInit(&argc, argv);                        // glut 초기화
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
    glutInitWindowPosition(100, 100);             // 윈도우의 위치 지정
    glutInitWindowSize(250, 250);                 // 윈도우의 크기 지정
    glutCreateWindow("Example1");                 // 윈도우 생성

    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) // glew 초기화
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";

    glutDisplayFunc(drawScene); // 출력 함수의 지정
    glutReshapeFunc(Reshape);   // 다시 그리기 함수 지정
    glutKeyboardFunc(Keyboard); // 키보드 입력 콜백함수 지정
    glutMainLoop();             // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
    // 변경된 배경색 설정
    glClearColor(g_color[0], g_color[1], g_color[2], g_color[3]);
    glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
    glutSwapBuffers();            // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) //--- 콜백 함수: 키보드 입력 콜백 함수
{
    switch (key)
    {
    case 'R':
    case 'r':
        g_color[0] = 1.0f;
        g_color[1] = 0.0f;
        g_color[2] = 0.0f;
        break;

    case 'G':
    case 'g':
        g_color[0] = 0.0f;
        g_color[1] = 1.0f;
        g_color[2] = 0.0f;
        break;

    case 'B':
    case 'b':
        g_color[0] = 0.0f;
        g_color[1] = 0.0f;
        g_color[2] = 1.0f;
        break;

    case 'A':
    case 'a':
        g_color[0] = randomColor(gen);
        g_color[1] = randomColor(gen);
        g_color[2] = randomColor(gen);
        break;

    case 'W':
    case 'w':
        g_color[0] = 1.0f;
        g_color[1] = 1.0f;
        g_color[2] = 1.0f;
        break;

    case 'K':
    case 'k':
        g_color[0] = 0.0f;
        g_color[1] = 0.0f;
        g_color[2] = 0.0f;
        break;

    case 'T':
    case 't':
        glutTimerFunc(10, colorChangeTimer, 1);
        g_colorChagneTimer = true;
        break;

    case 'S':
    case 's':
        g_colorChagneTimer = false;
        break;

    case 'Q':
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void colorChangeTimer(int value)
{
    if (g_colorChagneTimer)
    {
        g_color[0] = randomColor(gen);
        g_color[1] = randomColor(gen);
        g_color[2] = randomColor(gen);
        glutPostRedisplay();
        glutTimerFunc(500, colorChangeTimer, 1);
    }
}