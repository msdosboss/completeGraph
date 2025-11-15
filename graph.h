#ifndef GRAPH_H
#define GRAPH_H

#include "cglm/cglm.h"
#include <math.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define DEGTORADCON M_PI / 180.0f

typedef struct {
  vec2 coords;
} Vertex;

const char *helloWorld();
Vertex *createVertices(int n);
void printVertices(Vertex *vertices, int n);

#endif
