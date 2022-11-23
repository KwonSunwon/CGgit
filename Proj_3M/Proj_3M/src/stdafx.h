#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib ")

#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <stdlib.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>
#include <algorithm>
#include <time.h>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace std;

#define BASE_SIZE 100.f
#define GRAVITY 1.f

#define PERSP 0
#define ORTHO 1

#define LEFT 0b0001
#define RIGHT 0b0010
#define UP 0b0100
#define DOWN 0b1000

#define THIRD 0
#define FIRST 1