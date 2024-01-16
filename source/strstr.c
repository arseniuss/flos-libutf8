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
#include <string.h>

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
            return (char *)p;

    return NULL;
}
