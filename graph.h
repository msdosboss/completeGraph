#ifndef GRAPH_H
#define GRAPH_H

#include "cglm/cglm.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600
#define DEGTORADCON M_PI / 180.0f

struct Vertex {
  int id;
  vec2 coords;
  struct Vertex **edges;
  int *edgeWeights;
  int edgeCount;
};

typedef struct Vertex Vertex;

const char *helloWorld();
Vertex *createVertices(int n);
void printVertices(Vertex *vertices, int n);
Vertex *processMatrix(int **matrix, int len);
Vertex *processMatrixJt(int **adjacencyMatrix, int **weightMatrix, int len);
void graphCleanup(Vertex *graphVertices, int len);

#endif
