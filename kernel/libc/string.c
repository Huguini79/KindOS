#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* str)
{
	size_t i = 0;

	while (str[i] != '\0')
	{
		i++;
	}

	return i;
	
}

int strcmp(const char* str1, const char* str2)
{
	unsigned char u1, u2;
	while (1)
	{
		u1 = (unsigned char)*str1++;
		u2 = (unsigned char)*str2++;
		if (u1 != u2)
		{
			return u1 - u2;
		}

		if (u1 == '\0')
		{
			return 0;
		}
	}

	return 0;
}