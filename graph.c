#include "graph.h"

const char *helloWorld(){
    return "Hello World!\n";
}


// This creates a pointer that needs to be freed
Vertex *createVertices(int n){
    Vertex *vertices = malloc(sizeof(Vertex) * n);
    float step = 360.0f / (float)n;
    float location = 0.0f;
    for(int i = 0; i < n; i++){
        float locationRad = location * DEGTORADCON;
        vertices[i].coords[0] = cos(locationRad); //coords[0] is x
        vertices[i].coords[1] = sin(locationRad); //coords[1] is y
        // convert coords to screen space
        float radius = 250.0f;
        vertices[i].coords[0] = vertices[i].coords[0] * radius + WIDTH / 2.0f;
        vertices[i].coords[1] = vertices[i].coords[1] * radius + HEIGHT / 2.0f;
        location += step;
		vertices[i].edgeCount = 0; //added this to support processMatrix -cwb-
    }
    return vertices;
}


void printVertices(Vertex *vertices, int n){
    for(int i = 0; i < n; i++){
        printf("vertices[%d].x = %f vertices[%d].y = %f\n", i, vertices[i].coords[0], i, vertices[i].coords[1]);
    }
}

//Note: this hard-coded array is temporary - eventually, will be parsed in somehow
int cool_array[5][5] = {
	{0, 1, 0, 1, 0},
	{1, 0, 0, 1, 1},
	{0, 0, 0, 1, 1},
	{1, 1, 1, 0, 0},
	{0, 1, 1, 0, 0}
};

int processMatrix(int **matrix, int len)
{
	int edge_count = 0;
    int edgeCountVertex[len];
    memset(edgeCountVertex, 0, len);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (matrix[i][j])
				edge_count += matrix[i][j];
                edgeCountVertex[i]++;
		}
	}
	printf("DEBUG: edge count: %d\n", edge_count);
	Vertex *graph_vertices = createVertices(len); //the actual vertices
	Vertex **edge_vertices = (Vertex **) malloc(sizeof(Vertex *) * edge_count); //the edges between above vertices
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (matrix[i][j]) {
				printf("DEBUG[%d][%d]\n", i, j);
				fflush(stdout);

				graph_vertices[i].edges = edge_vertices - 1 + edge_count--; // :)
				*(graph_vertices[i].edges) = &graph_vertices[j];
				graph_vertices[i].edgeCount++; //edgeCount is initialized to 0 in createVertices.
			}
		}
	}
	//edge_count should be exactly 0 here
	//All the graph_vertices should have fully set up arrays of Vertex **
	//testing is probably in order

	//DEBUG code below
	for (int i = 0; i < len; i++) {
		printf("Vertex %d with memory address %#X:\nNo. of edges: %d\n", i, graph_vertices + i, graph_vertices->edgeCount);
		for (int j = 0; j < len; j++) {
			if (j < graph_vertices[i].edgeCount)
				printf("\tPoints to: %#X\n", graph_vertices[i].edges[j]);
			else break;
		}
	}
}

//can remove this later.
void debugWrapper(void) {
	int **cool_array_fixed = malloc(sizeof(int *) * 5);
	for (int i = 0; i < 5; i++) {
		cool_array_fixed[i] = (int *) malloc(sizeof(int) * 5);
		for (int j = 0; j < 5; j++)
			cool_array_fixed[i][j] = cool_array[i][j];
	}
	processMatrix(cool_array_fixed, 5);

	for (int i = 0; i < 5; i++) {
		free(cool_array_fixed[i]);
	}
	free(cool_array_fixed);
}
