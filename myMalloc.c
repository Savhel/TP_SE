//
// Created by PFI BERTO COMPUTER on 01/11/2024.
//
#include <stddef.h>
#include <unistd.h>

// Définir une taille de bloc de mémoire
#define BLOCK_SIZE 1024

// Structure pour représenter un bloc de mémoire
typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static Block* freeList = NULL;

// Fonction pour fusionner les blocs adjacents libres
void fusion() {
    Block* curr = freeList;
    while (curr && curr->next) {
        if ((char*)curr + sizeof(Block) + curr->size == (char*)curr->next) {
            curr->size += curr->next->size + sizeof(Block);
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

// Implémentation de malloc
void* myMalloc(size_t size) {
    Block* curr;
    if (!freeList) {
        curr = (Block*)sbrk(BLOCK_SIZE);
        curr->size = BLOCK_SIZE - sizeof(Block);
        curr->free = 1;
        curr->next = NULL;
        freeList = curr;
    }

    curr = freeList;
    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return (char*)curr + sizeof(Block);
        }
        curr = curr->next;
    }

    curr = (Block*)sbrk(size + sizeof(Block));
    if ((void*)curr == (void*)-1) {
        return NULL; // Pas assez de mémoire
    }
    curr->size = size;
    curr->free = 0;
    curr->next = NULL;

    return (char*)curr + sizeof(Block);
}

// Implémentation de free
void myFree(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->free = 1;
    fusion();
}
