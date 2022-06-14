/* See LICENSE file for copyright and license details. */
#include "type.h"

int
char1cmp(const void *v1, const void *v2) {
    Char r1 = *(Char *) v1;
    Char r2 = *(Char *) v2;

    return r1 - r2;
}

int
char2cmp(const void *v1, const void *v2) {
    Char r = *(Char *) v1;
    Char *p = (Char *) v2;

    return r >= p[0] && r <= p[1] ? 0 : r - p[0];
}
