CC = g++
OBJ = main.o format_input.o

shell379: shell.out
	./shell.out

shell.out: $(OBJ)
	$(CC) -o shell.out $(OBJ) -g
main.o: 
	$(CC) -c main.cpp
format_input.o:
	$(CC) -c format_input.cpp
clean:
	rm -f shell.out $(OBJ)