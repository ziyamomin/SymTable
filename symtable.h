/* symbtable.h */

#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED

#include <stddef.h>

typedef struct SymTable *SymTable_T;

/* Returns a new SymTable object that contains no bindings or NULL if there is insufficient memory available.*/
SymTable_T SymTable_new(void);

/* Frees all memory occupied by oSymTable parameter */
void SymTable_free(SymTable_T oSymTable);

/* Returns the number of bindings in oSymTable parameter */
size_t SymTable_getLength(SymTable_T oSymTable);

/* If oSymTable parameter does not contain a binding with pcKey, then SymTable_put must add a new binding to oSymTable consisting of key pcKey and value pvValue and return 1 (TRUE). Otherwise the function must leave oSymTable unchanged and return 0 (FALSE). If insufficient memory is available, then the function must leave oSymTable unchanged and return 0 (FALSE). */
int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/* If oSymTable parameter contains a binding with key pcKey, then SymTable_replace must replace the binding's value with pvValue and return the old value. Otherwise it must leave oSymTable unchanged and return NULL. */
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/* Returns the value of the binding within oSymTable parameter whose key is pcKey or NULL if no such binding exists */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* Returns the value of the binding within oSymTable whose key is pcKey or NULL if no such binding exists. */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* Removes the bidning from oSymTable and returns the binding's value. Otherwise, the function does not change oSymTable parameter and returns NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* Applies a given function (pfApply) to each binding stored within the oSymTable parameter. */
void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);

#endif
