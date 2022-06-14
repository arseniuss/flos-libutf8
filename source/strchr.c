/* See LICENSE file for copyright and license details. */
#include <string.h>
#include "utf8.h"

char *
utf8_strchr(const char *s, utf8_char r) {
    char buf[utf8_max + 1];
    int n;

    if (r < utf8_single)
        return strchr(s, r);

    if (!(n = utf8_chr2str(buf, &r)))
        return NULL;

    buf[n] = '\0';
    return strstr(s, buf);
}
