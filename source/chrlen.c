/* See LICENSE file for copyright and license details. */
#include "utf8.h"

int
utf8_chrlen(utf8_char r) {
    if (!utf8_isvalid(r))
        return 0;
    else if (r < UTF8_CHAR(1) << 7)
        return 1;
    else if (r < UTF8_CHAR(1) << 11)
        return 2;
    else if (r < UTF8_CHAR(1) << 16)
        return 3;
    else if (r < UTF8_CHAR(1) << 21)
        return 4;
    else if (r < UTF8_CHAR(1) << 26)
        return 5;
    else
        return 6;
}

size_t
utf8_nchrlen(const utf8_char *p, size_t len) {
    size_t i, k = 0;

    for (i = 0; i < len; i++)
        k += utf8_chrlen(*p++);

    return k;
}
