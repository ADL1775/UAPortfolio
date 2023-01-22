CC=gcc

CFLAGS+=-Wall -g

EXECUTABLE=films

OBJECTS=list.o

all: $(OBJECTS) films.c
	$(CC) $(CFLAGS) $(OBJECTS) films.c -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXECUTABLE)
