
ifeq ($(OS),Windows_NT)
all: main.o graph.o glad.o
	gcc main.o graph.o glad.o -o main -lm -lglfw3 --define-macro $(OS)
else
all: main.o graph.o glad.o
	gcc main.o graph.o glad.o -o main -lm -lglfw --define-macro $(OS)
endif

glad.o: glad.c
	gcc -c glad.c -o glad.o --define-macro $(OS)

main.o: main.c
	gcc -c main.c -o main.o --define-macro $(OS)

graph.o: graph.c
	gcc -c graph.c -o graph.o --define-macro $(OS)

clean:
	rm -f *.o main
