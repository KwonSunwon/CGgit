#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib ")

#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#define LEFT_TOP 0
#define RIGHT_TOP 1
#define LEFT_BOTTOM 2
#define RIGHT_BOTTOM 3

#define LEFT 10
#define RIGHT 11
#define TOP 12
#define BOTTOM 13

using namespace std;
using namespace glm;