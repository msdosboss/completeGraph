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
    }
    return vertices;
}


void printVertices(Vertex *vertices, int n){
    for(int i = 0; i < n; i++){
        printf("vertices[%d].x = %f vertices[%d].y = %f\n", i, vertices[i].coords[0], i, vertices[i].coords[1]);
    }
}
