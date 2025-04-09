CFLAGS = -std=c++11 -Wall -Wextra
CC = g++

SRCS = huffman.cpp BitReader.cpp BitWriter.cpp
OBJS = $(SRCS:.cpp=.o)

huffman: ${OBJS} cli.o
	${CC} ${CFLAGS} $^ -o $@

cli.o: cli.cpp
	${CC} ${CFLAGS} cli.cpp -c

%.o: %.cpp %.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm huffman treeOut ${OBJS} cli.o uncomp.txt