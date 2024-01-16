/* See LICENSE file for copyright and license details. */
#include "utf8.h"

size_t
utf8_strnlen(const char *s, size_t len) {
    unsigned char c, i, m, n, x;
    const char *p;
    size_t k;
    utf8_char r;

    for (k = 0; *(p = s) != '\0'; len -= s - p, k++) {
        if (len == 0) /* can't even look at s[0] */
            return k;

        c = *s++;

        if ((c & 0300) != 0300) /* not a leading byte */
            continue;

        n = utf8_tbl[c & 077];

        if (n == 0) /* illegal byte */
            continue;

        if (len == 1) /* reached len limit */
            return k;

        if ((*s & 0300) != 0200) /* not a continuation byte */
            continue;

        x = 0377 >> n;
        r = c & x;

        r = (r << 6) | (*s++ & 077);

        if (r <= x) /* overlong sequence */
            continue;

        m = (len < n) ? len : n;

        for (i = 2; i < m; i++) {
            if ((*s & 0300) != 0200) /* not a continuation byte */
                break;

            s++;
        }

        if (i < m) /* broke out of loop early */
            continue;

        if (i < n) /* must have reached len limit */
            return k;
    }

    return k;
}

size_t
utf8_strlen(const char *s) {
    return utf8_strnlen(s, (size_t)-1);
}
