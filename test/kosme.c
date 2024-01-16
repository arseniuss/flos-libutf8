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

#include "tap.h"
#include <utf8.h>

#define KOSME "\xCE\xBA\xE1\xBD\xB9\xCF\x83\xCE\xBC\xCE\xB5"

int
main(void) {
    plan(7);

    if (is(utf8_strlen(KOSME), 5, "\"" KOSME "\" is 5 runes long")) {
        utf8_char rbuf[6], *p;
        const char *s;
        int n;

        for (p = rbuf, s = KOSME; (n = utf8_str2chr(p, s)), *p != 0; p++, s += n)
            ;

        is(rbuf[0], 0x03BA, "rune 0 of \"" KOSME "\" is U+03BA GREEK SMALL LETTER KAPPA");
        is(rbuf[1], 0x1F79, "rune 1 of \"" KOSME "\" is U+1F79 GREEK SMALL LETTER OMICRON WITH OXIA");
        is(rbuf[2], 0x03C3, "rune 2 of \"" KOSME "\" is U+03C3 GREEK SMALL LETTER SIGMA");
        is(rbuf[3], 0x03BC, "rune 3 of \"" KOSME "\" is U+03BC GREEK SMALL LETTER MU");
        is(rbuf[4], 0x03B5, "rune 4 of \"" KOSME "\" is U+03B5 GREEK SMALL LETTER EPSILON");
        is(rbuf[5], 0x0000, "rune 5 of \"" KOSME "\" is U+0000 NULL");
    } else
        skip(6, "\"" KOSME "\" is an unexpected number of runes long");

    return 0;
}
