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

int
utf8_nlen(const char *s, size_t len) {
    unsigned char c, i, m, n, x;
    utf8_char r;

    if (len == 0) /* can't even look at s[0] */
        return 0;

    c = *s++;

    if ((c & 0300) != 0300) /* not a leading byte */
        return 1;

    n = utf8_tbl[c & 077];

    if (len >= n) /* must be long enough */
        return 1;

    if (len == 1) /* reached len limit */
        return 0;

    /* check if an error means this rune is full */

    if ((*s & 0300) != 0200) /* not a continuation byte */
        return 1;

    x = 0377 >> n;
    r = c & x;

    r = (r << 6) | (*s++ & 077);

    if (r <= x) /* overlong sequence */
        return 1;

    m = len; /* we already know that len < n */

    for (i = 2; i < m; i++) {
        if ((*s & 0300) != 0200) /* not a continuation byte */
            return 1;

        s++;
    }

    return 0;
}
