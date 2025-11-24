#ifndef MAIN_H
#define MAIN_H

#include "graph.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unistd.h>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
GLFWwindow *initWindow();
unsigned int initVAO(Vertex *vertices, int *indicies, int n, unsigned int *EBO);
char *readShaderFile(const char *fileName);
unsigned int linkShaders(const char *vertexFileName,
                         const char *fragmentFileName);
void processInput(GLFWwindow *window);
int *edgesIndicies(int n);

#endif
