
CC=g++
basic: main.o
	$(CC) main.o -o basic
main.o: src/main.cpp
	$(CC) -c src/main.cpp
clean:
	rm ./*.o
