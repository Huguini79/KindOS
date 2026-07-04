#include "types.h"
#include "console.h"
#include "string.h"

#include <stdint.h>
#include <stddef.h>

void printk(const char* str)
{
	size_t len = strlen(str);

	for (size_t i = 0; i < len; ++i)
	{
		put_c(str[i]);
	}

}