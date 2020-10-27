INCL    := $(wildcard include/*.h)
SRC     := $(subst include/,,$(INCL:%.h=src/%.c))
OBJ     := $(subst src/,,$(SRC:%.c=bin/%.o))
LIBS    := 
EXE     := prog

CC      := gcc
CFLAGS  := -ansi -pedantic -Wall -O2
LIBPATH := -L.
LDFLAGS := -o bin/$(EXE) $(LIBPATH) $(LIBS)
RM      := rm -f

bin/%.o: src/%.c include/%.h
	$(CC) -c $(CFLAGS) src/$*.c include/$*.h
	@mv $*.o bin/$*.o

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ)

$(OBJ): $(INCL)

clean:
	$(RM) $(OBJ) $(INCL:.h=.h.gch)
