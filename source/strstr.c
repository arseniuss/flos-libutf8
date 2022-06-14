/* See LICENSE file for copyright and license details. */
#include <string.h>
#include "utf8.h"

char *
utf8_strstr(const char *s1, const char *s2) {
    const char *p;
    int n1, n2;
    utf8_char r;

    n1 = utf8_str2chr(&r, s2);
    if (r < utf8_single)
        return strstr(s1, s2);

    n2 = strlen(s2);
    for (p = s1; (p = utf8_strchr(p, r)); p += n1)
        if (!strncmp(p, s2, n2))
            return (char *) p;

    return NULL;
}

