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

#include <flos/utf8.h>

#include "tap.h"

#define CHECK(S, N, RS)                                                                                                \
    do {                                                                                                               \
        if (is(utf8_strlen(S), (N), RS " is " #N " runes long")) {                                                     \
            utf8_char r;                                                                                               \
            int i;                                                                                                     \
            const char *p = (S);                                                                                       \
            for (i = 0; *p != '\0'; i++) {                                                                             \
                p += utf8_str2chr(&r, p);                                                                              \
                if (r != utf8_error)                                                                                   \
                    break;                                                                                             \
            }                                                                                                          \
            is(i, (N), RS " read as in error");                                                                        \
        } else                                                                                                         \
            skip(1, #S " is an unexpected number of runes long");                                                      \
    } while (0)

int
main(void) {
    plan(30);

    CHECK("\xC0\xAF", 2, "2-byte overlong U+002F SLASH");
    CHECK("\xE0\x80\xAF", 2, "3-byte overlong U+002F SLASH");
    CHECK("\xF0\x80\x80\xAF", 3, "4-byte overlong U+002F SLASH");
    CHECK("\xF8\x80\x80\x80\xAF", 4, "5-byte overlong U+002F SLASH");
    CHECK("\xFC\x80\x80\x80\x80\xAF", 5, "6-byte overlong U+002F SLASH");

    CHECK("\xC1\xBF", 2, "2-byte overlong U+0000007F DELETE");
    CHECK("\xE0\x9F\xBF", 2, "3-byte overlong U+000007FF");
    CHECK("\xF0\x8F\xBF\xBF", 3, "4-byte overlong U+0000FFFF <not a character>");
    CHECK("\xF8\x87\xBF\xBF\xBF", 4, "5-byte overlong U+001FFFFF <not a character>");
    CHECK("\xFC\x83\xBF\xBF\xBF\xBF", 5, "6-byte overlong U+03FFFFFF <not a character>");

    CHECK("\xC0\x80", 2, "2-byte overlong U+0000 NULL");
    CHECK("\xE0\x80\x80", 2, "3-byte overlong U+0000 NULL");
    CHECK("\xF0\x80\x80\x80", 3, "4-byte overlong U+0000 NULL");
    CHECK("\xF8\x80\x80\x80\x80", 4, "5-byte overlong U+0000 NULL");
    CHECK("\xFC\x80\x80\x80\x80\x80", 5, "6-byte overlong U+0000 NULL");

    return 0;
}
