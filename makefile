all: main.o graph.o glad.o
	gcc main.o graph.o glad.o -o main -lm -lglfw

glad.o: glad.c
	gcc -c glad.c -o glad.o

main.o: main.c
	gcc -c main.c -o main.o

graph.o: graph.c
	gcc -c graph.c -o graph.o

clean:
	rm -f *.o main
