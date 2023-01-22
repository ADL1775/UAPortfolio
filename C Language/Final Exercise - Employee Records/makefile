CC=gcc -lpthread

CFLAGS=-Wall -g

OBJECTS=

CFILE = FinalExercise_3.c

EXECUTABLE=Final3

all: $(OBJECTS) $(CFILE)
	$(CC) $(CFLAGS) $(OBJECTS) $(CFILE) -o $(EXECUTABLE) -lm
	
.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.o
	rm -f $(EXECUTABLE)