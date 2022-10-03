CC = g++
OBJ = main.o format_input.o process.o process_table.o execute.o

shell379: $(OBJ)
	$(CC) -o shell379 $(OBJ) -g
main.o: 
	$(CC) -c main.cpp
format_input.o:
	$(CC) -c format_input.cpp
process.o:
	$(CC) -c process.cpp

process_table.o:
	$(CC) -c process_table.cpp

execute.o :
	$(CC) -c execute.cpp
clean:
	rm -f shell379 $(OBJ)