# Macros
CC = gcc217
# CC = gcc217m
CFLAGS =
# CFLAGS = -g
# CFLAGS = -D NDEBUG
# CFLAGS = -D NDEBUG -O

# Dependency rules for non-file targets
all: testsymtablelist
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f testsymtablelist *.o

# Dependency rules for file targets
testsymtablelist: testsymtablelist.o symtablelist.o
	$(CC) $(CFLAGS) testsymtablelist.o symtablelist.o -o testsymtablelist

testsymtablelist.o: testsymtablelist.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablelist.c

symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c