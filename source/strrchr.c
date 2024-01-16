/* See LICENSE file for copyright and license details. */
#include "utf8.h"
#include <string.h>

char *
utf8_strrchr(const char *s, utf8_char r) {
    const char *p = NULL;
    utf8_char r0;
    int n;

    if (r < utf8_single)
        return strrchr(s, r);

    for (; *s != '\0'; s += n) {
        n = utf8_str2chr(&r0, s);
        if (r == r0)
            p = s;
    }
    return (char *)p;
}
