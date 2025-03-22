#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h" 


enum { BUCKET_COUNT = 509 };

struct Binding {
 const char *key;
 int value;
 struct Binding *next;
};
struct SymTable {
 struct Binding *buckets[BUCKET_COUNT];
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (!oSymTable) return NULL;
    oSymTable->head = NULL;
    oSymTable->length = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable);
    
    struct SymTableNode *currentNode;
    struct SymTableNode *nextNode;
    
    currentNode = oSymTable->head;
    while (currentNode) {
        nextNode = currentNode->next;
        free(currentNode->pcKey);
        free(currentNode);
        currentNode = nextNode;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable);
    return oSymTable->length * oSymTable->numberOfBuckets;
}

/* Need to mod hash function number by bucket count and use that result to find the proper bucket - placed at the end if the key is not already in it */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    assert(oSymTable);
    assert(pcKey))
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;

    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            return 0;
        }
        currentNode = currentNode->next;
    }

    struct SymTableNode *newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
    if (!newNode) return 0;

    newNode->pcKey = (char*)malloc(strlen(pcKey) + 1);
    if (!newNode->pcKey) {
        free(newNode);
        return 0;
    }
    strcpy(newNode->pcKey, pcKey);

    newNode->pvValue = (void *)pvValue;
    newNode->next = oSymTable->head;
    oSymTable->head = newNode;
    oSymTable->length++;

    return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    assert(oSymTable);
    assert(pcKey);
    
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;
    
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
    assert(cKey);
    
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;
    
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            return 1;
        }
        currentNode = currentNode->next;
    }
    return 0;
}

/* Hash function is deterministic so a key would map to the same hash 
each time; again mod the hash by the bucket count to get the bucket,
iterate through until key is possibly found - runs in linear time */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable);
    assert(pcKey);
    
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;
    
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            return currentNode->pvValue;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable)
    assert(pcKey);
    
    struct SymTableNode *currentNode;
    struct SymTableNode *prevNode;
    
    currentNode = oSymTable->head;
    prevNode = NULL;

    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            if (prevNode) {
                prevNode->next = currentNode->next;
            } else {
                oSymTable->head = currentNode->next;
            }
            void *value = currentNode->pvValue;
            free(currentNode->pcKey);
            free(currentNode);
            oSymTable->length--;
            return value;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra) {
    assert(oSymTable);
    assert(pfApply);
    
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;
    
    while (currentNode) {
        pfApply(currentNode->pcKey, currentNode->pvValue, (void *)pvExtra);
        currentNode = currentNode->next;
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