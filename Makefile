CFLAGS = -std=c++11 -Wall -Wextra
CC = g++

SRCS = huffman.cpp BitReader.cpp BitWriter.cpp
OBJS = $(SRCS:.cpp=.o)

huffman: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@

%.o: %.cpp %.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm huffman treeOut ${OBJS}