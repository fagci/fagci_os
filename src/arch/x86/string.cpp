#include "string.h"

size_t strlen(const char* string) {
    size_t result = 0;
    while (string[result])
        result++;
    return result;
}

size_t strcmp(const char *str1, const char *str2) {
	for(size_t i = 0; ; ++i) {
		if(str1[i] != str2[i])
			return i + 1;

		if(str1[i] == 0)
			return 0;
	}
}

void itoa(char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* If %d is specified and D is minus, put `-' in the head.  */
    if (base == 'd' && d < 0) {
            *p++ = '-';
            buf++;
            ud = -d;
    } else if (base == 'x') {
            divisor = 16;
    }

    /* Divide UD by DIVISOR until UD == 0.  */
    do {
            int remainder = ud % divisor;

            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    /* Terminate BUF.  */
    *p = 0;

    /* Reverse BUF.  */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
    }
}
