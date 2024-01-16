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

#include <stdlib.h>

#define LEN(X) (sizeof(X) / sizeof *(X))

int
ISCHAR(utf8_char r) {
    utf8_char *match;

#ifdef RANGES
    if ((match = bsearch(&r, RANGES, LEN(RANGES), sizeof *RANGES, char2cmp)))
        return 1;
#endif
#ifdef LACES1
    if ((match = bsearch(&r, LACES1, LEN(LACES1), sizeof *LACES1, char2cmp)))
        return !((r - match[0]) % 2);
#endif
#ifdef LACES2
    if ((match = bsearch(&r, LACES2, LEN(LACES2), sizeof *LACES2, char2cmp)))
        return !((r - match[0]) % 2);
#endif
#ifdef SINGLES
    if ((match = bsearch(&r, SINGLES, LEN(SINGLES), sizeof *SINGLES, char1cmp)))
        return 1;
#endif

    return 0;
}

#ifdef TOCHAR

utf8_char
TOCHAR(utf8_char r) {
    utf8_char *match;

    #ifdef RANGES
    if ((match = bsearch(&r, RANGES, LEN(RANGES), sizeof *RANGES, char2cmp)))
        return match[2] + (r - match[0]);
    #endif
    #ifdef LACES1
    if ((match = bsearch(&r, LACES1, LEN(LACES1), sizeof *LACES1, char2cmp)))
        return ((r - match[0]) % 2) ? r : r + 1;
    #endif
    #ifdef LACES2
    if ((match = bsearch(&r, LACES2, LEN(LACES2), sizeof *LACES2, char2cmp)))
        return ((r - match[0]) % 2) ? r : r - 1;
    #endif
    #ifdef SINGLES
    if ((match = bsearch(&r, SINGLES, LEN(SINGLES), sizeof *SINGLES, char1cmp)))
        return match[1];
    #endif

    return r;
}
#endif

#ifdef TOINT
    #ifdef RANGES

int
TOINT(utf8_char c) {
    utf8_char *match;

    if ((match = bsearch(&c, RANGES, LEN(RANGES), sizeof *RANGES, char2cmp)))
        return c - match[0];

    return -1;
}
    #endif
#endif
