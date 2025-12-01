#ifndef GRAPH_H
#define GRAPH_H

#include "cglm/cglm.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

struct TableEntry {
	struct TableEntry *next;
	int cumulative_weight;
	Vertex *node;
};

const char *helloWorld();
Vertex *createVertices(int n);
void printVertices(Vertex *vertices, int n);
Vertex *processMatrixJT(int **adjacencyMatrix, int **weightMatrix, int len);
void graphCleanup(Vertex *graphVertices, int len);
int **createAdjcencyMatrixCompleteGraph(int n);
int **createAdjcencyMatrixRandomGraph(int n);
struct TableEntry *solvePath (Vertex *graph, int len, int dest_id);
struct TableEntry *findListEnd(struct TableEntry *list);

#endif
