CFLAGS=-Wall -g
# add -g for debugging with gdb

ifeq ($(OS),Windows_NT)
all: main.o graph.o glad.o openGLFunctions.o
	gcc $(CFLAGS) main.o graph.o glad.o openGLFunctions.o -o main -lm -lglfw3 --define-macro $(OS)
else
all: main.o graph.o glad.o openGLFunctions.o
	gcc $(CFLAGS) main.o graph.o glad.o openGLFunctions.o -o main -lm -lglfw
endif

ifeq ($(OS),Windows_NT)
glad.o: glad.c
	gcc $(CFLAGS) -c glad.c -o glad.o --define-macro $(OS)
else
glad.o: glad.c
	gcc $(CFLAGS) -c glad.c -o glad.o
endif

ifeq ($(OS),Windows_NT)
main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o --define-macro $(OS)
else
main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o
endif

ifeq ($(OS),Windows_NT)
graph.o: graph.c
	gcc $(CFLAGS) -c graph.c -o graph.o --define-macro $(OS)
else
graph.o: graph.c
	gcc $(CFLAGS) -c graph.c -o graph.o
endif

ifeq ($(OS),Windows_NT)
openGLFunctions.o: openGLFunctions.c
	gcc $(CFLAGS) -c openGLFunctions.c -o openGLFunctions.o --define-macro $(OS)
else
openGLFunctions.o: openGLFunctions.c
	gcc $(CFLAGS) -c openGLFunctions.c -o openGLFunctions.o
endif

clean:
	rm -f *.o main
