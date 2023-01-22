
CC=gcc

CFLAGS+=-Wall -g

EXECUTABLE=10_3

OBJECTS=Exercise10_3.o

all: $(OBJECTS) Exercise10_3.c
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXECUTABLE)
