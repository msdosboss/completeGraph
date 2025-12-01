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
    Vertex *vertices = processMatrixJT(cool_array_fixed, weight_array_fixed, 5);

	for (int i = 0; i < 5; i++) {
		free(cool_array_fixed[i]);
	}
	free(cool_array_fixed);
	for (int i = 0; i < 5; i++) {
		free(weight_array_fixed[i]);
	}
	free(weight_array_fixed);

	struct TableEntry *path_result = solvePath(vertices, 5, 4);
	printf("Cumulative weight of solved path: %d\n", findListEnd(path_result)->cumulative_weight);	
	struct TableEntry *current_path_point = path_result;
	while (current_path_point->next != NULL) {
		printf("Node #%d\n", current_path_point->node->id);
		current_path_point = current_path_point->next;
	}


    return vertices;
}

// *graph is the starting Vertex
struct TableEntry *solvePath (Vertex *graph, int len, int dest_id)
{
	struct TableEntry table[len][len];
	for (int i = 0; i < len; i++) { 
		for (int j = 0; j < len; j++) {
			table[i][j].next = NULL;
			table[i][j].cumulative_weight = 0;
			table[i][j].node = NULL;
		}
	}
	Vertex *sets[len];
	for (int i = 0; i < len; i++)
		sets[i] = NULL;
	int set_idx = 0;
	sets[set_idx] = graph;
	for (; set_idx < len; set_idx++) {
		for (int i = 0; i < sets[set_idx]->edgeCount; i++) {
			struct TableEntry *current_entry = &table[set_idx][sets[set_idx]->edges[i]->id];
			int table_weight_ref = (set_idx == 0) ? 0 : findListEnd(&table[set_idx - 1][sets[set_idx]->id])->cumulative_weight;
			current_entry->next = malloc(sizeof(struct TableEntry));
			current_entry->next->next = NULL;
			current_entry->next->cumulative_weight = \
			table_weight_ref + sets[set_idx]->edgeWeights[i];
			current_entry->next->node = sets[set_idx];
			current_entry = current_entry->next;
		}
		struct TableEntry *smallest_entry = NULL;
		for (int i = 0; i < len; i++) {
			if (findListEnd(&table[set_idx][i])->node == NULL) {
				continue;
			}
			for (int j = 0; j <= set_idx; j++) {
				if (&graph[i] == sets[j])
					goto duplicate;	
				;
			}
			if (smallest_entry == NULL) {
				smallest_entry = &table[set_idx][i];
				sets[set_idx + 1] = &graph[i];
				continue;
			}
			
			struct TableEntry *end = findListEnd(smallest_entry);
			struct TableEntry *table_end = findListEnd(&table[set_idx][i]);
			if (end->cumulative_weight > table_end->cumulative_weight) {
				smallest_entry = &table[set_idx][i];
				sets[set_idx + 1] = &graph[i];
				printf("DEBUG - replacing smallest entry with %#X\n", &table[set_idx][i]);
				fflush(stdout);
			}

			duplicate:
				continue;
		}
		struct TableEntry *small_end = findListEnd(smallest_entry);
		if (small_end->node->id == dest_id) {
			for (int ii = 0; ii < len; ii++) {
				for (int jj = 0; jj < len; jj++) {
					printf("orca16: %d %d\t", findListEnd(&table[ii][jj])->cumulative_weight,
						(findListEnd(&table[ii][jj])->node) == NULL ? -99 : findListEnd(&table[ii][jj])->node->id
					);
				}
				putchar('\n');
			}
			return smallest_entry->next;
		}
	}

	//TODO - actually free the memory
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

