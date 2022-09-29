#pragma warning(disable:4996)

#include <iostream>
#include <random>

#include <stdlib.h>
#include <stdio.h>

#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

char* filetobuf(char* file);

void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

GLuint shaderID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

GLclampf g_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
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
    glutInitWindowSize(800, 600);                 // 윈도우의 크기 지정
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

    make_vertexShaders(); //--- 버텍스 세이더 만들기
    make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
    shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기


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

    glUseProgram(shaderID);

    glPointSize(100.0);
    glDrawArrays(GL_POINTS, 0, 1);

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

void make_vertexShaders()
{
    GLchar* vertexSource;
    //--- 버텍스 세이더 읽어 저장하고 컴파일 하기
    //--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
    char file[] = "vertex.glsl";
    vertexSource = filetobuf(file);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
        return;
    }
}

void make_fragmentShaders()
{
    GLchar* fragmentSource;
    //--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
    char file[] = "fragment.glsl";
    fragmentSource = filetobuf(file); // 프래그세이더 읽어오기
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
        return;
    }
}

GLuint make_shaderProgram()
{
    GLuint ShaderProgramID;
    ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기
    glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
    glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
    glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
    glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
    glDeleteShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
    if (!result) {
        glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
        cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
        return false;
    }
    glUseProgram(ShaderProgramID); //--- 만들어진 세이더 프로그램 사용하기
    //--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
    //--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
    //--- 사용하기 직전에 호출할 수 있다.
    return ShaderProgramID;
}

char* filetobuf(char* file)
{
    FILE* fptr;
    long length;
    char* buf;
    fptr = fopen(file, "rb"); // Open file for reading 
    if (!fptr) // Return NULL on failure 
        return NULL;
    fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
    length = ftell(fptr); // Find out how many bytes into the file we are 
    buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
    fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
    fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
    fclose(fptr); // Close the file 
    buf[length] = 0; // Null terminator 
    return buf; // Return the buffer 
}