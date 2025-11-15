#ifndef MAIN_H
#define MAIN_H

#include "graph.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
GLFWwindow *initWindow();
unsigned int initVAO(Vertex *vertices, int n);
char *readShaderFile(const char *fileName);
unsigned int linkShaders(const char *vertexFileName,
                         const char *fragmentFileName);
void processInput(GLFWwindow *window);

#endif
