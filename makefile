
CC=g++
tiffor: main.o
	$(CC) main.o -g -o /data/data/com.termux/files/home/.hadat/bin/tiffor
main.o: main.cpp
	$(CC) -c main.cpp
clean:
	rm main.o