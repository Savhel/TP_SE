//
// Created by PFI BERTO COMPUTER on 01/11/2024.
//
#include <stddef.h>
#include <unistd.h>

// Taille du bloc de mémoire
#define BLOCK_SIZE 1024

// Structure pour représenter un bloc de mémoire
typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static Block* freeList = NULL;

// Fonction pour initialiser la liste de mémoire
void initialize() {
    freeList = (Block*)sbrk(BLOCK_SIZE);
    freeList->size = BLOCK_SIZE - sizeof(Block);
    freeList->free = 1;
    freeList->next = NULL;
}

// Fonction pour fusionner les blocs libres adjacents
void fuisionner() {
    Block* curr = freeList;
    while (curr && curr->next) {
        if (curr->free && curr->next->free && (char*)curr + sizeof(Block) + curr->size == (char*)curr->next) {
            curr->size += curr->next->size + sizeof(Block);
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

// Implémentation de malloc
void* monMalloc(size_t size) {
    if (!freeList) {
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

    Block* newBlock = (Block*)sbrk(size + sizeof(Block));
    if ((void*)newBlock == (void*)-1) {
        return NULL; // Pas assez de mémoire
    }
    newBlock->size = size;
    newBlock->free = 0;
    newBlock->next = NULL;
    return (char*)newBlock + sizeof(Block);
}

// Implémentation de free
void monFree(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->free = 1;
    fuisionner();
}
