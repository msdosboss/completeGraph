#ifndef OPENGL_FUNCTIONS_H
#define OPENGL_FUNCTIONS_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
GLFWwindow *initWindow(int width, int height);
char *readShaderFile(const char *fileName);
unsigned int linkShaders(const char *vertexFileName,
                         const char *fragmentFileName);

#endif
