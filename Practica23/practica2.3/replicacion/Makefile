CC=g++
CFLAGS=-g -I.
DEPS = Socket.h UDPServer.h Chat.h
OBJ = Socket.o UDPServer.o
LIBS=-lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

cs: $(OBJ) ChatServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

all: cs cc

.PHONY: clean

clean:
	rm -f *.o cs cc

