# Macros
CC = gcc217m
# CC = gcc217
CFLAGS = -g
# CFLAGS = -D NDEBUG
# CFLAGS = -D NDEBUG -O

# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f testsymtablelist testsymtablehash *.o meminfo*.out

# Dependency rules for file targets
testsymtablelist: testsymtable.o symtablelist.o
	$(CC) $(CFLAGS) testsymtable.o symtablelist.o -o testsymtablelist

testsymtable.o: testsymtable.c symtable.h
	$(CC) $(CFLAGS) -c testsymtable.c

symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c

testsymtablehash: testsymtable.o symtablehash.o
	$(CC) $(CFLAGS) testsymtable.o symtablehash.o -o testsymtablehash

testsymtablehash.o: testsymtablehash.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablehash.c

symtablehash.o: symtablehash.c symtable.h
	$(CC) $(CFLAGS) -c symtablehash.c