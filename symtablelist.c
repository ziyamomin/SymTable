/* symtablelist.c */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"
#include <string.h>

/* Definition of a SymTable (symbol table) data structure */
struct SymTable {
    struct SymTableNode *firstNode;
    size_t length;
};

/* Defintion of a SymTableNode data structure */
struct SymTableNode {
    char *pcKey;
    void *pcValue;
    struct SymTableNode *nextNode;
};


SymTable_T SymTable_new(void) {
    SymTable_T oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));

    if(oSymTable == NULL) {
        return NULL;
    }
    oSymTable->firstNode = NULL;
    oSymTable->length = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct SymTableNode *currentNode; 
    struct SymTableNode *nextNode;
    assert(oSymTable != NULL);
    currentNode = oSymTable->firstNode;
    while(currentNode != NULL) {
        nextNode = currentNode->nextNode;
        free(currentNode->pcKey);
        free(currentNode);
        currentNode = nextNode;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->length;

}

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue) {
        assert(oSymTable != NULL);
        assert(pcKey != NULL);
        assert(pcValue != NULL);

        if(SymTable_contains(oSymTable, pcKey)) {
            return 0;
        }

        /* Creating a defensive copy */
        keyCopy = (char *)malloc(strlen(pcKey) + 1);
        if(keyCopy == NULL) {
            return 0;
        }
        strcpy(keyCopy, pcKey);

        newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
        if(newNode == NULL) {
            free(keyCopy);
            return 0;
        }

        newNode->pcKey = keyCopy;
        newNode->pvValue = (void*)pvValue;
        newNode->nextNode = oSymTable->firstNode;
        oSymTable->firstNode = newNode; 
        oSymTable->length++;
        return 1;
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

    struct SymTableNode *currentNode;

    currentNode = oSymTable->firstNode;
    while(currentNode != NULL) {
        if(strcmp(currentNode->pcKey, pcKey) == 0) {
            return 1;
        }
        currentNode = currentNode->nextNode;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    struct SymTableNode *currentNode;

    currentNode = oSymTable->firstNode;
    while(currentNode != NULL) {
        if(strcmp(currentNode->pcKey, pcKey) == 0) {
            return currentNode->pvValue;
        }
        currentNode;
    }

    return NULL;

}


void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    struct SymTableNode *currentNode;
    struct SymTableNode *previousNode;
    void *pvValue;

    previousNode = NULL;
    currentNode = oSymTable->firstNode;

    while(currentNode != NULL) {
        if(strcmp(psCurrentNode->pcKey, pcKey) == 0) {
            pcValue = currentNode->pvValue;

            if(previousNode == NULL) {
                oSymTable->firstNode = currentNode->nextNode;
            }
            else {
                previousNode->nextNode = currentNode->nextNode;
            }

            free(currentNode->pcKey);
            free(currentNode);
            oSymTable-->length--;
            return pvValue;
        }
        previousNode = currentNode;
        currentNode = currentNode->nextNode; 
    }

    return NULL;

}

void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra) {
    struct SymTableNode *currentNode;
    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    currentNode = oSymTable->firstNode;
    while (currentNode != NULL) {
        (*pfApply)(currentNode->pcKey, currentNode->pvValue, (void*)pvExtra);
        currentNode = currentNode->nextNode;
    }
}

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive.  Adapted from the COS 217 lecture notes. */

static int SymTable_hash(const char *pcKey, int iBucketCount)
{
   enum {HASH_MULTIPLIER = 599};
   int i;
   unsigned int uiHash = 0U;
   assert(pcKey != NULL);
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}