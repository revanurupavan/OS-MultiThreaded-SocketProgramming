all: program1 program2

program1:
	gcc server.c -o server.o

program2:
	gcc client.c -o client.o

clean:
	rm -f program1 program2 *.o
