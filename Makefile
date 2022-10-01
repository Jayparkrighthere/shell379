CC = g++
OBJ = main.o process_input.o

shell379: shell.out
	./shell.out

shell.out: $(OBJ)
	$(CC) -o shell.out $(OBJ) -g
main.o: 
	$(CC) -c main.cpp
process_input.o:
	$(CC) -c process_input.cpp
clean:
	rm -f shell.out $(OBJ)