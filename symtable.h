/* symbtable.h */

#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED
#include <stddef.h>

/* Defines a new type, SymTable_T, as a pointer to a struct SymTable.
This represents a symbold table, with SymTable_T
acting as an alias for a pounter to the structure. */
typedef struct SymTable *SymTable_T;

/* Returns a new SymTable object that contains no bindings or NULL if
there is insufficient memory available.*/
SymTable_T SymTable_new(void);

/* Frees all memory occupied by oSymTable */
void SymTable_free(SymTable_T oSymTable);

/* Returns the number of bindings in oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable);

/* If the oSymTable parameter does not contain a binding with the pcKey,
then SymTable_put adds a new binding to oSymTable consisting of key
pcKey and value pvValue and return 1 (TRUE). Otherwise, oSymTable is
left unchanged and 0 (FALSE) is returned. If insufficient memory is
available, then oSymTable is left unchanged and 0 (FALSE) is
returned. */
int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/* If oSymTable parameter contains a binding with key pcKey, then
SymTable_replace replaces the binding's value with pvValue and return
the old value. Otherwise, oSymTable is left unchanged and NULL is
returned. */
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/* Returns 1 (TRUE) if oSymTable contains pcKey or 0 (FALSE) if no
such binding exists */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* Returns the value of the binding pcKey within oSymTable whose key
is pcKey or NULL if no such binding exists. */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* Removes the binding pcKey from oSymTable and returns the binding's
value. Otherwise, the function does not change oSymTable parameter
and returns NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* Applies the given function pfApply to each binding stored within
the oSymTable parameter. The function (pfApply) is called for wach
key-value pair in the symbol table. 
The pvExtra parameter is passed to pfApply as an additional argument
for extra, custom processing. */
void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount);

#endif