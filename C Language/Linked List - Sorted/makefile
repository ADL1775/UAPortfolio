
CC=gcc

CFLAGS+=-Wall -g

EXECUTABLE=9_2

OBJECTS=Exercise9_2.o

all: $(OBJECTS) Exercise9_1.c
	$(CC) $(CFLAGS) $(OBJECTS) Exercise9_2.c -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXECUTABLE)
