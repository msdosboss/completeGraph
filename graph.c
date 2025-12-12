#include "graph.h"

const char *helloWorld(){
    return "Hello World!\n";
}


int **createAdjcencyMatrixRandomGraph(int n){
    srand(time(NULL));
    int **matrix = malloc(sizeof(int *) * n);
    for(int i = 0; i < n; i++){
        matrix[i] = malloc(sizeof(int) * n);
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            // 25% chance
            if(((rand() % 4) == 0) && i < j){
                matrix[i][j] = 1;
            } else if (i != j) {
				matrix[i][j] = matrix[j][i];
			} else {
                matrix[i][j] = 0;
            }
        }
    }

	//DEBUG stuff
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!(matrix[i][j] == matrix[j][i])) {
				fprintf(stderr, "Failed in createAdjcencyMatrixRandomGraph()\n");
			}
		}
	}

    return matrix;
}


int **createAdjcencyMatrixCompleteGraph(int n){
    int **matrix = malloc(sizeof(int *) * n);
    for(int i = 0; i < n; i++){
        matrix[i] = malloc(sizeof(int) * n);
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i != j){
                matrix[i][j] = 1;
            }
            else{
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}


// This creates a pointer that needs to be freed
Vertex *createVertices(int n){
    Vertex *vertices = malloc(sizeof(Vertex) * n);
    float step = 360.0f / (float)n;
    float location = 0.0f;
    for(int i = 0; i < n; i++){
        // Added id attrib to make it easier to identify each vertex
        vertices[i].id = i;
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

int weight_matrix[5][5] = {
    {0, 2, 0, 5, 0},
    {2, 0, 0, 7, 5},
    {0, 0, 0, 12, 11},
    {5, 7, 12, 0, 0},
    {0, 5, 11, 0, 0}
};


void graphCleanup(Vertex *graphVertices, int len){
    for(int i = 0; i < len; i++){
        free(graphVertices[i].edges);
        free(graphVertices[i].edgeWeights);
    }
    free(graphVertices);
}

Vertex *processMatrixJT(int **adjacencyMatrix, int **weightMatrix, int len)
{
    // Creates The Vertices that make up the graph
    Vertex *graphVertices = createVertices(len);
    // Count the amount of edges for each vertex
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            if(adjacencyMatrix[i][j]){
                graphVertices[i].edgeCount++;
            }
        }
    }

    // Malloc the edge points and weights
    for(int i = 0; i < len; i++){
        graphVertices[i].edges = malloc(sizeof(Vertex *) * graphVertices[i].edgeCount);
        graphVertices[i].edgeWeights = malloc(sizeof(int) * graphVertices[i].edgeCount);
    }

    for(int i = 0; i < len; i++){
        int idx = 0;
        for(int j = 0; j < len; j++){
            if(adjacencyMatrix[i][j]){
                // setting up weights and edges
                // edges will have their weights be on the same idx
                graphVertices[i].edges[idx] = &graphVertices[j];
                graphVertices[i].edgeWeights[idx++] = weightMatrix[i][j];
            }
        }
    }

    return graphVertices;
}


//can remove this later.
Vertex *debugWrapper(void) {
	int **cool_array_fixed = malloc(sizeof(int *) * 5);
	for (int i = 0; i < 5; i++) {
		cool_array_fixed[i] = (int *) malloc(sizeof(int) * 5);
		for (int j = 0; j < 5; j++)
			cool_array_fixed[i][j] = cool_array[i][j];
	}
	int **weight_array_fixed = malloc(sizeof(int *) * 5);
	for (int i = 0; i < 5; i++) {
		weight_array_fixed[i] = (int *) malloc(sizeof(int) * 5);
		for (int j = 0; j < 5; j++)
			weight_array_fixed[i][j] = weight_matrix[i][j];
	}
    Vertex *vertices = processMatrixJT(cool_array_fixed, cool_array_fixed, 5);

	int *solved = solvePathTwo(vertices, 5, 4);
	for (int i = 0; solved[i] != -1; i++) {
		printf("i: %d solved[i] %d\n", i, solved[i]);
	}

	for (int i = 0; i < 5; i++) {
		free(cool_array_fixed[i]);
	}
	free(cool_array_fixed);
	for (int i = 0; i < 5; i++) {
		free(weight_array_fixed[i]);
	}
	free(weight_array_fixed);

    return vertices;
}


//return pointer needs to be free'd
int *solvePathTwo(Vertex *graph, int len, int dest_id)
{
    // Stores the current best-known distance to each vertex.
    int dist[len];
    // Stores the predecessor vertex on the shortest path
    int prev[len];
    // Stores previous set information
    int set[len];
	for (int i = 0; i < len; i++) {
		dist[i] = -1;
		prev[i] = -1;
        set[i] = 0;
	}
    set[0] = 1;
	dist[0] = 0;

    int currentIdx = 0;
    while (!set[dest_id]){
        int ec = graph[currentIdx].edgeCount;
        for (int i = 0; i < ec; i++){
            //Vertex *currentEdge = graph[currentIdx].edges[i];
            int currentEdgeId = graph[currentIdx].edges[i]->id;
            int currentEdgeWeight = graph[currentIdx].edgeWeights[i];
            if ((dist[currentEdgeId] > currentEdgeWeight + dist[graph[currentIdx].id] || dist[currentEdgeId] == -1) && !set[currentEdgeId]){
                prev[currentEdgeId] = graph[currentIdx].id;
                dist[currentEdgeId] =  currentEdgeWeight + dist[graph[currentIdx].id];
            }
        }
        int weightMin = -1;
        int newSetIdx = -1;
        for (int i = 0; i < len; i++){
			if (weightMin != -1 && dist[i] == -1) continue;
            if(!set[i] && (weightMin > dist[i] || weightMin == -1)){
                weightMin = dist[i];
                newSetIdx = i;
            }
        }
        //set[newSetIdx] = 1;
		set[currentIdx] = 1;
		currentIdx = newSetIdx;
    }

	int *result = malloc(len * sizeof(int));
	//memset(result, -1, len);
	for (int i = 0; i < len; i++) {
		result[i] = -1;
	}
	int result_counter = 0;
	int idx = dest_id;
	while (idx != 0) {
		result[result_counter++] = prev[idx];
		idx = prev[idx];
	}

	int left = 0;
	int right = result_counter - 1;

	while (left < right) {
		int tmp = result[left];
		result[left++] = result[right];
		result[right--] = tmp;
	}
	result[result_counter] = dest_id;

    return result;
}


struct TableEntry *findListEnd(struct TableEntry *list)
{
	if (list == NULL) {
		fprintf(stderr, "NULL pointer passed to findListEnd()\n");
		return NULL;
	}
	struct TableEntry *current = list;
	while (current->next != NULL)
		current = current->next;

	return current;
}
