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
utf8_chrlen(utf8_char r) {
    if (!utf8_isvalid(r))
        return 0;
    else if (r < UTF8_CHAR(1) << 7)
        return 1;
    else if (r < UTF8_CHAR(1) << 11)
        return 2;
    else if (r < UTF8_CHAR(1) << 16)
        return 3;
    else if (r < UTF8_CHAR(1) << 21)
        return 4;
    else if (r < UTF8_CHAR(1) << 26)
        return 5;
    else
        return 6;
}

size_t
utf8_nchrlen(const utf8_char *p, size_t len) {
    size_t i, k = 0;

    for (i = 0; i < len; i++)
        k += utf8_chrlen(*p++);

    return k;
}
