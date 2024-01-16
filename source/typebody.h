#include <stdlib.h>

#define LEN(X) (sizeof(X) / sizeof *(X))

int
ISCHAR(utf8_char r) {
    utf8_char *match;

#ifdef RANGES
    if ((match = bsearch(&r, RANGES, LEN(RANGES), sizeof *RANGES, char2cmp)))
        return 1;
#endif
#ifdef LACES1
    if ((match = bsearch(&r, LACES1, LEN(LACES1), sizeof *LACES1, char2cmp)))
        return !((r - match[0]) % 2);
#endif
#ifdef LACES2
    if ((match = bsearch(&r, LACES2, LEN(LACES2), sizeof *LACES2, char2cmp)))
        return !((r - match[0]) % 2);
#endif
#ifdef SINGLES
    if ((match = bsearch(&r, SINGLES, LEN(SINGLES), sizeof *SINGLES, char1cmp)))
        return 1;
#endif

    return 0;
}

#ifdef TOCHAR

utf8_char
TOCHAR(utf8_char r) {
    utf8_char *match;

    #ifdef RANGES
    if ((match = bsearch(&r, RANGES, LEN(RANGES), sizeof *RANGES, char2cmp)))
        return match[2] + (r - match[0]);
    #endif
    #ifdef LACES1
    if ((match = bsearch(&r, LACES1, LEN(LACES1), sizeof *LACES1, char2cmp)))
        return ((r - match[0]) % 2) ? r : r + 1;
    #endif
    #ifdef LACES2
    if ((match = bsearch(&r, LACES2, LEN(LACES2), sizeof *LACES2, char2cmp)))
        return ((r - match[0]) % 2) ? r : r - 1;
    #endif
    #ifdef SINGLES
    if ((match = bsearch(&r, SINGLES, LEN(SINGLES), sizeof *SINGLES, char1cmp)))
        return match[1];
    #endif

    return r;
}
#endif

#ifdef TOINT
    #ifdef RANGES

int
TOINT(utf8_char c) {
    utf8_char *match;

    if ((match = bsearch(&c, RANGES, LEN(RANGES), sizeof *RANGES, char2cmp)))
        return c - match[0];

    return -1;
}
    #endif
#endif
