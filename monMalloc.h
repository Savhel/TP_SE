//
// Created by PFI BERTO COMPUTER on 01/11/2024.
//

#ifndef MONMALLOC_H
#define MONMALLOC_H
#include  <stddef.h>

void* monMalloc(size_t size);
void monFree(void* ptr);
#endif //MONMALLOC_H
