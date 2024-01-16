/* See LICENSE file for copyright and license details. */
#include "type.h"

int
char1cmp(const void *v1, const void *v2) {
    utf8_char r1 = *(utf8_char *) v1;
    utf8_char r2 = *(utf8_char *) v2;

    return r1 - r2;
}

int
char2cmp(const void *v1, const void *v2) {
    utf8_char r = *(utf8_char *) v1;
    utf8_char *p = (utf8_char *) v2;

    return r >= p[0] && r <= p[1] ? 0 : r - p[0];
}
