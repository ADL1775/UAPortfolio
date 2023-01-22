

CFLAGS+=-Wall -g

EXECUTABLE=Exercise6_5

OBJECTS=Item.o Player.o Location.o Game.o ItemList.o

SHARED=Item.so Player.so Location.so Game.so ItemList.so

all: $(OBJECTS) lib main.c
	gcc $(CFLAGS) $(OBJECTS) main.c -o $(EXECUTABLE)
	ar rc adventure.a $(OBJECTS)

lib: $(OBJECTS)
#	gcc -shared

slib: $(SHARED) slibrary
	gcc -c -Wall -Werror -fpic $(OBJECTS)

clean:
	rm -f *.o
	rm -f *.so
	rm -f *~
	rm -f $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) -c $<

.c.so:
	$(CC) -c -fPIC -shared %<
