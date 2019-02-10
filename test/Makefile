# sudo apt-get install check valgrind

# définition des cibles particulières
.PHONY: clean, compilation

# désactivation des règles implicites
.SUFFIXES:
.SUFFIXES: .c .o

# constantes
CFLAGS = -g -no-pie -I../include
LDFLAGS = -lm -lcheck -lsubunit -pthread \
  -lrt -lsubunit -lm

OBJ = check.o
EXEC = check

all: compilation $(EXEC)

%.o: %.c
	gcc -Wall $(CFLAGS) -c $<

$(EXEC): $(OBJ) ../lib/util.o ../lib/fon.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

compilation:
	make -C ..

memoryLeaks:
	valgrind --leak-check=full -q --undef-value-errors=no ./$(EXEC)

clean:
	rm -f *.o check
