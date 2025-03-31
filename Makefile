CFLAGS = -std=c++11

test: BitWriter.o test.o
	g++ ${CFLAGS} BitWriter.o test.o -o test

test2: BitReader.o test2.o
	g++ ${CFLAGS} BitReader.o test2.o -o test2

BitReader.o: BitReader.cpp
	g++ ${CFLAGS} BitReader.cpp -c

main: main.o BitWriter.o
	g++ ${CFLAGS} main.o BitWriter.o -o main

main.o: main.cpp Bitwriter.h
	g++ ${CFLAGS} main.cpp -c

BitWriter.o: BitWriter.cpp BitWriter.h
	g++ ${CFLAGS} BitWriter.cpp -c

test.o: test.cpp BitWriter.h
	g++ ${CFLAGS} test.cpp -c

test2.o: test2.cpp
	g++ ${CFLAGS} test2.cpp -c

clean:
	rm test test2 main *.o

