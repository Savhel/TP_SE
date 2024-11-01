//
// Created by PFI BERTO COMPUTER on 01/11/2024.
//
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 1024

char memory[MEMORY_SIZE];

typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static Block* freeList = (Block*)memory;

void initialize() {
    freeList->size = MEMORY_SIZE - sizeof(Block);
    freeList->free = 1;
    freeList->next = NULL;
}

void merge() {
    Block* curr = freeList;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += curr->next->size + sizeof(Block);
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

void* myMalloc(size_t size) {
    if (!freeList->size) {
        initialize();
    }

    Block* curr = freeList;
    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(Block)) {
                Block* newBlock = (Block*)((char*)curr + sizeof(Block) + size);
                newBlock->size = curr->size - size - sizeof(Block);
                newBlock->free = 1;
                newBlock->next = curr->next;
                curr->next = newBlock;
                curr->size = size;
            }
            curr->free = 0;
            return (char*)curr + sizeof(Block);
        }
        curr = curr->next;
    }

    return NULL; // Pas assez de mémoire
}

void myFree(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->free = 1;
    merge();
}
