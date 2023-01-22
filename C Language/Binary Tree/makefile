CC=gcc

CFLAGS+=-Wall -g

EXECUTABLE=BinaryTree

OBJECTS=Exercise7_4.o

all: $(OBJECTS) Exercise7_4.c
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXECUTABLE)
