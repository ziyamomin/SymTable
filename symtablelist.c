#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define the structure for the symbol table node */
struct SymTableNode {
    char *pcKey;
    void *pvValue;
    struct SymTableNode *next;
};

/* Define the structure for the symbol table */
struct SymTable {
    struct SymTableNode *head;
    int length;
};

/* Function to create a new symbol table */
struct SymTable* SymTable_new(void) {
    struct SymTable *oSymTable = (struct SymTable*)malloc(sizeof(struct SymTable));
    if (!oSymTable) return NULL;
    oSymTable->head = NULL;
    oSymTable->length = 0;
    return oSymTable;
}

/* Function to insert a key-value pair into the symbol table */
int SymTable_put(struct SymTable *oSymTable, const char *pcKey, const void *pvValue) {
    struct SymTableNode *newNode;
    char *keyCopy;

    if (!oSymTable || !pcKey) return 0;

    keyCopy = (char *)malloc(strlen(pcKey) + 1);
    if (!keyCopy) return 0;
    strcpy(keyCopy, pcKey);

    newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
    if (!newNode) {
        free(keyCopy);
        return 0;
    }

    newNode->pcKey = keyCopy;
    newNode->pvValue = (void *)pvValue;
    newNode->next = oSymTable->head;
    oSymTable->head = newNode;
    oSymTable->length++;

    return 1;
}

/* Function to check if a key exists in the symbol table */
int SymTable_contains(struct SymTable *oSymTable, const char *pcKey) {
    struct SymTableNode *currentNode;

    if (!oSymTable || !pcKey) return 0;

    currentNode = oSymTable->head;
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            return 1;
        }
        currentNode = currentNode->next;
    }
    return 0;
}

/* Function to get the value associated with a key */
void* SymTable_get(struct SymTable *oSymTable, const char *pcKey) {
    struct SymTableNode *currentNode;

    if (!oSymTable || !pcKey) return NULL;

    currentNode = oSymTable->head;
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            return currentNode->pvValue;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

/* Function to remove a key from the symbol table */
int SymTable_remove(struct SymTable *oSymTable, const char *pcKey) {
    struct SymTableNode *currentNode, *prevNode = NULL;
    void *pvValue;

    if (!oSymTable || !pcKey) return 0;

    currentNode = oSymTable->head;
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            if (prevNode) {
                prevNode->next = currentNode->next;
            } else {
                oSymTable->head = currentNode->next;
            }

            pvValue = currentNode->pvValue;
            free(currentNode->pcKey);
            free(currentNode);
            oSymTable->length--;
            return 1;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }
    return 0;
}

/* Function to apply a function to all elements in the table */
void SymTable_map(struct SymTable *oSymTable, void (*pfApply)(const char *, void *, void *), void *pvExtra) {
    struct SymTableNode *currentNode;

    if (!oSymTable || !pfApply) return;

    currentNode = oSymTable->head;
    while (currentNode) {
        (*pfApply)(currentNode->pcKey, currentNode->pvValue, pvExtra);
        currentNode = currentNode->next;
    }
}

/* Function to replace a value in the symbol table */
void* SymTable_replace(struct SymTable *oSymTable, const char *pcKey, const void *pvValue) {
    struct SymTableNode *currentNode;
    void *oldValue;

    if (!oSymTable || !pcKey) return NULL;

    currentNode = oSymTable->head;
    while (currentNode) {
        if (strcmp(currentNode->pcKey, pcKey) == 0) {
            oldValue = currentNode->pvValue;
            currentNode->pvValue = (void *)pvValue;
            return oldValue;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

/* Function to get the number of elements in the symbol table */
int SymTable_getLength(struct SymTable *oSymTable) {
    if (!oSymTable) return 0;
    return oSymTable->length;
}

/* Function to delete the entire symbol table */
void SymTable_free(struct SymTable *oSymTable) {
    struct SymTableNode *currentNode, *nextNode;

    if (!oSymTable) return;

    currentNode = oSymTable->head;
    while (currentNode) {
        nextNode = currentNode->next;
        free(currentNode->pcKey);
        free(currentNode);
        currentNode = nextNode;
    }

    free(oSymTable);
}
