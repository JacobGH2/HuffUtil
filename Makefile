CFLAGS = -std=c++11

test: BitWriter.o test.o
	g++ ${CFLAGS} BitWriter.o test.o -o test

main: main.o BitWriter.o
	g++ ${CFLAGS} main.o BitWriter.o -o main

main.o: main.cpp Bitwriter.h
	g++ ${CFLAGS} main.cpp -c

BitWriter.o: BitWriter.cpp BitWriter.h
	g++ ${CFLAGS} BitWriter.cpp -c

test.o: test.cpp BitWriter.h
	g++ ${CFLAGS} test.cpp -c

clean:
	rm test main *.o