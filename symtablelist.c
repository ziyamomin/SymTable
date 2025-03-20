// symtablelist.c

SymTable_T SymTable_new(void) {

}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable != NULL);

}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
}

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {
        assert(oSymTable != NULL);
        assert(pcKey != NULL);
        assert(pcValue != NULL);
     }

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {
        assert(oSymTable != NULL);
        assert(pcKey != NULL);
        assert(pcValue != NULL);

     }

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

}

void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra) {
        assert(oSymTable != NULL);
        assert(pcKey != NULL);
        assert(pcValue != NULL);
        assert(pvExtra != NULL);

     }

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive.  Adapted from the COS 217 lecture notes. */

static int SymTable_hash(const char *pcKey, int iBucketCount)
{
   enum {HASH_MULTIPLIER = 65599};
   int i;
   unsigned int uiHash = 0U;
   assert(pcKey != NULL);
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}