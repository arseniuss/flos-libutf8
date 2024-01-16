/* See LICENSE file for copyright and license details. */
#include "type.h"

int
char1cmp(const void *v1, const void *v2) {
    utf8_char r1 = *(utf8_char *)v1;
    utf8_char r2 = *(utf8_char *)v2;

    return r1 - r2;
}

int
char2cmp(const void *v1, const void *v2) {
    utf8_char r = *(utf8_char *)v1;
    utf8_char *p = (utf8_char *)v2;

    return r >= p[0] && r <= p[1] ? 0 : r - p[0];
}

int
utf8_isxdigit(utf8_char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int
utf8_tonum(utf8_char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return -1;
}
