#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
void* memset(void* s, int c, unsigned int len);
char* itoa(int value, char* str, int base);

#endif
