INCL    := $(wildcard include/*.h)
SRC     := src/main.c $(subst include/,,$(INCL:%.h=src/%.c))
OBJ     := $(subst src/,,$(SRC:%.c=bin/%.o))
LIBS    := -lm
EXE     := prog

CC      := gcc
CFLAGS  := -ansi -pedantic -Wall -O2
LIBPATH := -L.
LDFLAGS := -o bin/$(EXE) $(LIBPATH) $(LIBS)
RM      := rm -f

all: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)

bin/%.o: src/%.c include/%.h
	$(CC) -c $(CFLAGS) src/$*.c include/$*.h
	@mv $*.o bin/$*.o

bin/main.o: src/main.c
	$(CC) -c $(CFLAGS) src/main.c
	@mv main.o bin/main.o

$(OBJ): $(INCL)

clean:
	$(RM) $(OBJ) $(INCL:.h=.h.gch)
