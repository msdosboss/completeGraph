#ifndef MAIN_H
#define MAIN_H

#include "graph.h"
#include "openGLFunctions.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unistd.h>

unsigned int initVAO(Vertex *vertices, int *indicies, int n, unsigned int *EBO);
void processInput(GLFWwindow *window);
int *edgesIndicies(int n);
void drawGraph(GLFWwindow *window, unsigned int point_shader_program,
               unsigned int edge_shader_program, unsigned int VAO,
               unsigned int EBO, unsigned int len, unsigned int total_edges);
unsigned int totalEdges(Vertex *vertices, int len);
unsigned int *edgesIndiciesNew(Vertex *vertices, int len);
unsigned int initVAONew(Vertex *vertices, int len, unsigned int *EBO);

#endif
