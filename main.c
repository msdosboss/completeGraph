#include "main.h"

void debugWrapper(void); //function prototype - defined in graph.c - temporary only

int main(int argc, char **argv){
    int n;
    if(argc != 2){
        n = 20;
    }
    else{
        n = atoi(argv[1]);
    }

	debugWrapper();
	getchar(); //for the pause


    GLFWwindow *window;
    if((window = initWindow()) == NULL){
        return -1;
    }
    unsigned int shaderProgram = linkShaders("shaders/graphVertex.glsl", "shaders/graphFragments.glsl");
    glUseProgram(shaderProgram);

    unsigned int edgeShaderProgram = linkShaders("shaders/edgeVertex.glsl", "shaders/edgeFragments.glsl");

    //int *indicies = edgesIndicies(n);
    //Vertex *vertices = createVertices(n);

    //unsigned int VAO = initVAO(vertices, indicies, n);
    int osicaltionValue = 10;
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
    }
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
        graphicsIdx = 0;
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
        }
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


char *readShaderFile(const char *fileName){
    FILE* file = fopen(fileName, "r");
    if(file == NULL){
        printf("uhhhhhhhhhhhhhhhhhhhhh he's right behind isn't he?\n");
    }
    fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
    char *fileText = malloc(sizeof(char) * (size + 1));

    fread(fileText, 1, size,file);
    fileText[size] = '\0';

    fclose(file);
    return fileText;
}


unsigned int linkShaders(const char *vertexFileName, const char *fragmentFileName){
    GLchar *vertexFileText = readShaderFile(vertexFileName);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar * const*)&vertexFileText, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n",infoLog);
    }

    unsigned int fragmentShader;
    GLchar *fragmentFileText = readShaderFile(fragmentFileName);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar * const*)&fragmentFileText, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n",infoLog);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING_FAILED\n%s\n",infoLog);
    }

    free(vertexFileText);
    free(fragmentFileText);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


unsigned int initVAO(Vertex *vertices, int* indicies, int n, unsigned int *EBO){
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, n * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    int indiciesLen = ((n - 1) * n);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indiciesLen, indicies, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;

}


GLFWwindow *initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Complete Graph", NULL, NULL);
    if(window == NULL){
        printf("Failed to create GLFW window");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("GLAD failed\n");
        return NULL;
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

    glViewport(0, 0, fbWidth, fbHeight);

    //glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    return window;

}


void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}
