/* See LICENSE file for copyright and license details. */
#include "utf8.h"

int
utf8_chr2str(char *s, const utf8_char *p) {
    unsigned char i, n, x;
    utf8_char r = *p;

    n = utf8_chrlen(r);

    if (n == 1) {
        s[0] = r;
        return 1;
    }

    if (n == 0)
        return 0;

    for (i = n; --i > 0; r >>= 6)
        s[i] = 0200 | (r & 077);

    x = 0377 >> n;
    s[0] = ~x | r;

    return n;
}
