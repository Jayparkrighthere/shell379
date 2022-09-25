CC = g++


shell379: shell.out
	./shell.out

shell.out:
	$(CC) main.cpp -o shell.out
clean:
	rm -f shell.out