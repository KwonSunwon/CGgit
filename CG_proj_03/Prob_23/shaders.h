#ifndef __SHADERS_H__
#define __SHADERS_H__

#include "stdafx.h"

int makeVertexShader(const char *path);
int makeFragmentShader(const char *path);
GLuint initShader(const char *vertPath, const char *fragPath);

#endif