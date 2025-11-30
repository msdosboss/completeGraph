#include "main.h"

Vertex *debugWrapper(void); //function prototype - defined in graph.c - temporary only

int main(int argc, char **argv){
    int n;
    if(argc != 2){
        n = 20;
    }
    else{
        n = atoi(argv[1]);
    }

    int **matrix = createAdjcencyMatrixCompleteGraph(n);
    Vertex *completeVertices = processMatrixJT(matrix, matrix, n);
    unsigned int totalEdgesComplete = totalEdges(completeVertices, n);

    int **matrixRandom = createAdjcencyMatrixRandomGraph(n);
    Vertex *randomVertices = processMatrixJT(matrixRandom, matrixRandom, n);
    unsigned int totalEdgesRandom = totalEdges(randomVertices, n);

    Vertex *vertices = debugWrapper();
    unsigned int total_edges = totalEdges(vertices, 5);
	//getchar(); //for the pause


    GLFWwindow *window;
    if((window = initWindow(WIDTH, HEIGHT)) == NULL){
        return -1;
    }
    unsigned int shaderProgram = linkShaders("shaders/graphVertex.glsl", "shaders/graphFragments.glsl");
    glUseProgram(shaderProgram);

    unsigned int edgeShaderProgram = linkShaders("shaders/edgeVertex.glsl", "shaders/edgeFragments.glsl");


    unsigned int EBO;
    unsigned int VAO = initVAONew(vertices, 5, &EBO);

    unsigned int EBOComplete;
    unsigned int VAOComplete = initVAONew(completeVertices, n, &EBOComplete);

    unsigned int EBORandom;
    unsigned int VAORandom = initVAONew(randomVertices, n, &EBORandom);
    //int *indicies = edgesIndicies(n);
    //Vertex *vertices = createVertices(n);

    //unsigned int VAO = initVAO(vertices, indicies, n);
    /*int osicaltionValue = 10;
    unsigned int VAO[osicaltionValue];
    unsigned int EBO[osicaltionValue];
    int graphicsIdx = 0;
    for(int i = n - (osicaltionValue / 2); i < n + osicaltionValue / 2; i++){
        int *indicies = edgesIndicies(i);
        Vertex *vertices = createVertices(i);
        VAO[graphicsIdx] = initVAO(vertices, indicies, i, &(EBO[graphicsIdx]));
        graphicsIdx++;
        free(indicies);
        free(vertices);
    }*/
    mat4 projection;
    glm_mat4_identity(projection);
    glm_ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f, projection);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, (float*) projection);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glUseProgram(edgeShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(edgeShaderProgram, "projection"), 1, GL_FALSE, (float*) projection);

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);

        drawGraph(window, shaderProgram, edgeShaderProgram, VAO, EBO, 5, total_edges);
        sleep(1);
        drawGraph(window, shaderProgram, edgeShaderProgram, VAOComplete, EBOComplete, n, totalEdgesComplete);
        sleep(1);
        drawGraph(window, shaderProgram, edgeShaderProgram, VAORandom, EBORandom, n, totalEdgesRandom);
        sleep(1);

        /*graphicsIdx = 0;
        for(int i = n - osicaltionValue / 2; i < n + osicaltionValue / 2; i++){
            printf("n = %d\tDrawing %d vertices\t Drawing %d edges\n", i, i, ((i - 1) * i) / 2);
            glUseProgram(shaderProgram);
            glfwPollEvents();
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glBindVertexArray(VAO[graphicsIdx]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[graphicsIdx++]);
            glDrawArrays(GL_POINTS, 0, i);

            glUseProgram(edgeShaderProgram);
            glDrawElements(GL_LINES, ((i - 1) * i), GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(window);
            sleep(1);
            processInput(window);
        }*/
        /*glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, (float*) projection);
        glDrawArrays(GL_POINTS, 0, n);
        glUseProgram(edgeShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(edgeShaderProgram, "projection"), 1, GL_FALSE, (float*) projection);
        glDrawElements(GL_LINES, ((n  - 1) * n), GL_UNSIGNED_INT, 0);*/

        glfwSwapBuffers(window);
        processInput(window);
    }

    return 0;
}


void drawGraph(GLFWwindow *window, unsigned int point_shader_program, unsigned int edge_shader_program, unsigned int VAO, unsigned int EBO, unsigned int len, unsigned int total_edges){
    glUseProgram(point_shader_program);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawArrays(GL_POINTS, 0, len);

    glUseProgram(edge_shader_program);
    glDrawElements(GL_LINES, total_edges * 2, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

}


int *edgesIndicies(int n){
    // Number of edeges formula
    int *indicies = malloc(sizeof(int) * ((n - 1) * n));
    int idx = 0;
    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            indicies[idx++] = i;
            indicies[idx++] = j;
        }
    }
    return indicies;
}


void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        printf("orca\n");
    }
}


unsigned int totalEdges(Vertex *vertices, int len){
    unsigned int total_edges = 0;
    for(int i = 0; i < len; i++){
        total_edges += vertices[i].edgeCount;
    }
    // Edges are counted twice
    // Never rounds because of handshake therom
    // Also this assumes a simple graph
    // This will need to be changed if trying to do directional grpahs
    total_edges /= 2;
    return total_edges;
}


unsigned int *edgesIndiciesNew(Vertex *vertices, int len){
    unsigned int total_edges = totalEdges(vertices, len);
    // EBO needs two indicies per line
    unsigned int *indicies = malloc(sizeof(unsigned int) * total_edges * 2);

    // each edge is represented twice in the array
    int idx = 0;
    for(int i = 0; i < len; i++){
        int edgeCount = vertices[i].edgeCount;
        for(int j = 0; j < edgeCount; j++){
            Vertex *neighbor = vertices[i].edges[j];
            // Skips repeated edges
            if(vertices[i].id < neighbor->id){
                indicies[idx++] = vertices[i].id;
                indicies[idx++] = neighbor->id;
            }
        }
    }

    return indicies;
}


unsigned int initVAONew(Vertex *vertices, int len, unsigned int *EBO)
{
    unsigned int VAO;
    unsigned int VBO;

    unsigned int total_edges = totalEdges(vertices, len);
    // 2 floats per point (x, y)
    float *drawVertices = malloc(sizeof(float) * len * 2);

    int idx = 0;
    // loading point vertices
    for(int i = 0; i < len; i++){
        drawVertices[idx++] = vertices[i].coords[0];
        drawVertices[idx++] = vertices[i].coords[1];
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, len * 2 * sizeof(float), drawVertices, GL_STATIC_DRAW);

    unsigned int *indicies = edgesIndiciesNew(vertices, len);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * total_edges * 2, indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);

    free(drawVertices);
    free(indicies);

    return VAO;

}

// THIS FUNCTION NO LONGER WORKS
unsigned int initVAO(Vertex *vertices, int* indicies, int n, unsigned int *EBO){
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // This no longer works because Vertex struct contains more then just 2 floats now
    glBufferData(GL_ARRAY_BUFFER, n * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    int indiciesLen = ((n - 1) * n);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indiciesLen, indicies, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;

}
