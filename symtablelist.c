/*--------------------------------------------------------------------*/
/* symtablelist.c                                                     */
/* Author: Ziya Momin                                               */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h"

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
    /* The pointer to the first node (SymTableNode) in the symbol
    table */
    struct SymTableNode *head;
    /* The size of the symbol table (the number of nodes/entries in
    the symbol table) */
    size_t length;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (!oSymTable) return NULL;
    oSymTable->head = NULL;
    oSymTable->length = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable != NULL); 
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
    assert(oSymTable != NULL);
    return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const
void *pvValue) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
     
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;

    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            return 0;
        }
        currentNode = currentNode->next;
    }

    struct SymTableNode *newNode =
    (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
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

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    assert(pvValue != NULL);

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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
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

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    
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

void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra) {
    assert(oSymTable != NULL);
    assert(pfApply != NULL);
    assert(pvExtra != NULL);
    
    struct SymTableNode *currentNode;
    currentNode = oSymTable->head;
    
    while (currentNode) {
        pfApply(currentNode->pcKey, currentNode->pvValue,
        (void *)pvExtra);
        currentNode = currentNode->next;
    }
}
