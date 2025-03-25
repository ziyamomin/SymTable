#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h" 

/* Initial bucket count of the SymTable */
enum { BUCKET_COUNT = 509 };

struct SymTableNode {
    char *pcKey;
    void *pvValue;
    struct SymTableNode *next;
};


struct SymTable {
    struct SymTableNode **buckets;
    size_t bucketCount;
    size_t length;
    size_t expansionIndex;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    oSymTable->bucketCount = BUCKET_SIZES[0];
    oSymTable->buckets = (struct SymTableNode **)calloc(oSymTable->bucketCount, sizeof(struct SymTableNode*));
    if (!oSymTable->buckets) {
        free(oSymTable);
        return NULL;
    }
    oSymTable->length = 0;
    oSymTable->expansionIndex = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable);
    for (size_t i = 0; i < oSymTable->bucketCount; i++) {
        struct SymTableNode *current = oSymTable->buckets[i];
        while (current) {
            struct SymTableNode *next = current->next;
            free(current->pcKey);
            free(current);
            current = next;
        }
    }
    free(oSymTable->buckets);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable);
    return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    assert(oSymTable && pcKey);
    if (oSymTable->length >= oSymTable->bucketCount && oSymTable->expansionIndex + 1 < BUCKET_COUNT) {
        SymTable_expand(oSymTable);
    }
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct SymTableNode *current = oSymTable->buckets[index];
    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) return 0;
        current = current->next;
    }
    struct SymTableNode *newNode = (struct SymTableNode *)malloc(sizeof(struct SymTableNode));
    if (!newNode) return 0;
    newNode->pcKey = (char *)malloc(strlen(pcKey) + 1);
    if (!newNode->pcKey) {
        free(newNode);
        return 0;
    }
    strcpy(newNode->pcKey, pcKey);
    newNode->pvValue = (void *)pvValue;
    newNode->next = oSymTable->buckets[index];
    oSymTable->buckets[index] = newNode;
    oSymTable->length++;
    return 1;
}

/* need to modify what's below*/
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    assert(oSymTable);
    assert(pcKey);
    
    struct SymTableNode *currentNode;
    currentNode = oSymTable->buckets;
    
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            void *oldValue = currentNode->pvValue;
            currentNode->pvValue = (void *)pvValue;
            return oldValue;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable);
    assert(pcKey);
    return SymTable_get(oSymTable, pcKey) != NULL;
}
/* Hash function is deterministic so a key would map to the same hash 
each time; again mod the hash by the bucket count to get the bucket,
iterate through until key is possibly found - runs in linear time */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable);
    assert(pcKey);
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct SymTableNode *current = oSymTable->buckets[index];
    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) {
            return current->pvValue;
        }
        current = current->next;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable);
    assert(pcKey);
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct SymTableNode *current = oSymTable->buckets[index];
    struct SymTableNode *prev = NULL;
    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                oSymTable->buckets[index] = current->next;
            }
            void *value = current->pvValue;
            free(current->pcKey);
            free(current);
            oSymTable->length--;
            return value;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}


void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *, void *, void *), const void *pvExtra) {
    assert(oSymTable);
    assert(pfApply);
    for (size_t i = 0; i < oSymTable->bucketCount; i++) {
        struct SymTableNode *current = oSymTable->buckets[i];
        while (current) {
            pfApply(current->pcKey, current->pvValue, (void *)pvExtra);
            current = current->next;
        }
    }

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}