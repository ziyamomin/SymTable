# Macros
CC = gcc217
# CC = gcc217m
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
testsymtablelist: testsymtablelist.o symtablelist.o
	$(CC) $(CFLAGS) testsymtablelist.o symtablelist.o -o testsymtablelist

testsymtablelist.o: testsymtablelist.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablelist.c

symtablelist.o: symtablelist.c symtable.h
	$(CC) $(CFLAGS) -c symtablelist.c

symtablelist.o: testsymtablehash.o symtablehash.o
	$(CC) $(CFLAGS) -c testsymtablehash.o symtablehash.o -o testsymtablehash

testsymtablehash: testsymtablehash.o symtablehash.o 
	$(CC) $(CFLAGS) testsymtablehash.o symtablehash.o -o testsymtablehash

testsymtablehash.o: testsymtablehash.c symtable.h
	$(CC) $(CFLAGS) -c testsymtablehash.clean

symtablehash.o: symtablehash.c symtable.h
	$(CC) $(CFLAGS) -c symtablehash.c