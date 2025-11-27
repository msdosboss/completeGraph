
ifeq ($(OS),Windows_NT)
all: main.o graph.o glad.o
	gcc main.o graph.o glad.o -o main -lm -lglfw3 --define-macro $(OS)
else
all: main.o graph.o glad.o
	gcc main.o graph.o glad.o -o main -lm -lglfw
endif

ifeq ($(OS),Windows_NT)
glad.o: glad.c
	gcc -c glad.c -o glad.o --define-macro $(OS)
else
glad.o: glad.c
	gcc -c glad.c -o glad.o
endif

ifeq ($(OS),Windows_NT)
main.o: main.c
	gcc -c main.c -o main.o --define-macro $(OS)
else
main.o: main.c
	gcc -c main.c -o main.o
endif

ifeq ($(OS),Windows_NT)
graph.o: graph.c
	gcc -c graph.c -o graph.o --define-macro $(OS)
else
graph.o: graph.c
	gcc -c graph.c -o graph.o
endif

clean:
	rm -f *.o main
