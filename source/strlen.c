/*
 * flos/libutf8
 *
 * Copyright (C) 2012-2016 Connor Lane Smith <cls@lubutu.com>
 * Copyright (C) 2015 Laslo Hunhold <dev@frign.de>
 * Copyright (C) 2022-2024 Armands Arseniuss Skolmeisters <arseniuss@arseniuss.id.lv>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
