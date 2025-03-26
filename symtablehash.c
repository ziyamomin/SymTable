/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/* Author: Ziya Momin                                                 */
/*--------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h" 

/* Sequence of integers as bucket counts: 509, 1021, 2039, 4093, 8191,
16381, 32749, and 65521. These integers are primes that are close to
powers of two. */
static const size_t BUCKET_SIZES[] = {509, 1021, 2039, 4093, 8191,
16381, 32749, 65521};

/* A SymTableNode represents an entry in the symbol table */
struct SymTableNode {
    /* The pointer to a string in which the key is stored */
    char *pcKey;
    /* The pointer to a value associated with the key, which can
    point to any data type */
    void *pvValue;
    /* The pointer to the next node in the linked list of the
    symbol table */
    struct SymTableNode *next;
};


/* A SymTable consists of key-value pairs and link to the next node
in a linked list for chaining multiple entires together */
struct SymTable {
    /* A pointer to an array of buckets (linked lists) that store the
    key-value pairs in the symbol table. Each bucket corresponds to a
    hash index and contains a linked list of nodes. Each node in the
    list stores a key-value pair. */
    struct SymTableNode **buckets;
    /* The number of buckets/groups present in the symbol table. */
    size_t bucketCount;
    /* The size of the symbol table (the number of nodes/entries in
    the symbol table) */
    size_t length;
    /* An index that keeps track of the current expansion size for the
    hash table. It indicates which bucket size from the predefined list
    (e.g., 509, 1021, 2039) should be used when the table needs to
    grow. The expansion process helps keep the table's performance
    optimal as it scales. */
    size_t expansionIndex;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (!oSymTable) {
        return NULL;
    }

    oSymTable->bucketCount = BUCKET_SIZES[0];
    oSymTable->buckets = (struct SymTableNode **)
        calloc(oSymTable->bucketCount, sizeof(struct SymTableNode*));
    if (!oSymTable->buckets) {
        free(oSymTable);
        return NULL;
    }
    oSymTable->length = 0;
    oSymTable->expansionIndex = 0;
    return oSymTable;
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

/* This function increases oSymTable's bucket count to 1021 when it
exceeds 509. When the function detects that the new binding count
exceeds 1021, it increases the bucket count to 2039, etc. When
SymTable_put detects that the new binding count exceeds 65521, it
does not increase the bucket count. 65521 is the maximum number of
buckets that a SymTable object must contain. */
static void SymTable_expand(SymTable_T oSymTable) {
    size_t newBucketCount = BUCKET_SIZES[++oSymTable->expansionIndex];
    
    struct SymTableNode **newBuckets =
    (struct SymTableNode **)calloc(newBucketCount,
    sizeof(struct SymTableNode*));
    size_t i;
    

    assert(oSymTable != NULL);
    
    if (oSymTable->expansionIndex + 1 >=
    sizeof(BUCKET_SIZES)/sizeof(BUCKET_SIZES[0])) {
        return;
    }
    
    
    if (!newBuckets) {
        oSymTable->expansionIndex--;
        return;
    }
    
    for (i = 0; i < oSymTable->bucketCount; i++) {
        struct SymTableNode *current = oSymTable->buckets[i];
        while (current) {
            struct SymTableNode *next = current->next;
            
            size_t newIndex = SymTable_hash(current->pcKey,
            newBucketCount);
            
            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;
            
            current = next;
        }
    }
    
    free(oSymTable->buckets);
    oSymTable->buckets = newBuckets;
    oSymTable->bucketCount = newBucketCount;
}

void SymTable_free(SymTable_T oSymTable) {
    size_t i;

    assert(oSymTable != NULL);
    
    for (i = 0; i < oSymTable->bucketCount; i++) {
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
    assert(oSymTable != NULL);
    return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {
    size_t index;
    struct SymTableNode *newNode;
    struct SymTableNode *current;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    if (oSymTable->length >= oSymTable->bucketCount &&
    oSymTable->expansionIndex + 1 <
    sizeof(BUCKET_SIZES)/sizeof(BUCKET_SIZES[0])) {
        SymTable_expand(oSymTable);
    }

    index = SymTable_hash(pcKey, oSymTable->bucketCount);
    current = oSymTable->buckets[index];

    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) 
        { 
            return 0;
        }
        current = current->next;
    }

    newNode =
    (struct SymTableNode *)malloc(sizeof(struct SymTableNode));

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

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {
    struct SymTableNode *currentNode;
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    currentNode = oSymTable->buckets[index];

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
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
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct SymTableNode *current = oSymTable->buckets[index];

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct SymTableNode *current = oSymTable->buckets[index];

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) {
            return current->pvValue;
        }
        current = current->next;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    size_t index = SymTable_hash(pcKey, oSymTable->bucketCount);
    struct SymTableNode *current = oSymTable->buckets[index];
    struct SymTableNode *prev = NULL;
    void *value = NULL;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
    while (current) {
        if (strcmp(current->pcKey, pcKey) == 0) {
            value = current->pvValue;

            if (prev) {
                prev->next = current->next;
            } else {
                oSymTable->buckets[index] = current->next;
            }
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

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *,
void *, void *), const void *pvExtra) {
    size_t i;
    struct SymTableNode *current;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (i = 0; i < oSymTable->bucketCount; i++) {
        current = oSymTable->buckets[i];
        while (current) {
            pfApply(current->pcKey, current->pvValue, (void *)pvExtra);
            current = current->next;
        }
    }
}