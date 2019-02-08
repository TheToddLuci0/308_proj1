IDIR =../include
CC=gcc
DEPRICATION= -Wno-deprecated -Wno-deprecated-declarations

CFLAGS=-I$(IDIR) $(DEPRICATION)
ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = command_parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = command_parser.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
