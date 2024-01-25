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

#include <flos/utf8.h>

#define CHECK(S, N, R, RS)                                                                                             \
    do {                                                                                                               \
        utf8_char r;                                                                                                   \
        if (is(utf8_strlen(S), 1, #S " is 1 rune long")) {                                                             \
            is(utf8_str2chr(&r, (S)), (N), "rune in " #S " is " #N " bytes long");                                     \
            is(r, (R), "rune in " #S " is " RS);                                                                       \
        } else                                                                                                         \
            skip(2, #S " is an unexpected number of runes long");                                                      \
    } while (0)

int
main(void) {
    plan(50);

    {
        utf8_char r;
        is(utf8_str2chr(&r, ""), 1, "rune in \"\" is 1 byte long");
        is(r, UTF8_CHAR(0x0000), "rune in \"\" is U+0000 NULL");
    }

    CHECK("\xC2\x80", 2, UTF8_CHAR(0x00000080), "U+00000080 <control>");
    CHECK("\xE0\xA0\x80", 3, UTF8_CHAR(0x00000800), "U+00000800 SAMARITAN LETTER ALAF");
    CHECK("\xF0\x90\x80\x80", 4, UTF8_CHAR(0x00010000), "U+00010000 LINEAR B SYLLABLE B008 A");
    CHECK("\xF8\x88\x80\x80\x80", 5, utf8_error, "U+00200000 <not a character>");
    CHECK("\xFC\x84\x80\x80\x80\x80", 6, utf8_error, "U+04000000 <not a character>");

    CHECK("\x7F", 1, UTF8_CHAR(0x0000007F), "U+0000007F DELETE");
    CHECK("\xDF\xBF", 2, UTF8_CHAR(0x000007FF), "U+000007FF");
    CHECK("\xEF\xBF\xBF", 3, utf8_error, "U+0000FFFF <not a character>");
    CHECK("\xF7\xBF\xBF\xBF", 4, utf8_error, "U+001FFFFF <not a character>");
    CHECK("\xFB\xBF\xBF\xBF\xBF", 5, utf8_error, "U+03FFFFFF <not a character>");
    CHECK("\xFD\xBF\xBF\xBF\xBF\xBF", 6, utf8_error, "U+7FFFFFFF <not a character>");

    CHECK("\xED\x9F\xBF", 3, UTF8_CHAR(0x0000D7FF), "U+0000D7FF");
    CHECK("\xEE\x80\x80", 3, UTF8_CHAR(0x0000E000), "U+0000E000 <Private Use, First>");
    CHECK("\xEF\xBF\xBD", 3, UTF8_CHAR(0x0000FFFD), "U+0000FFFD REPLACEMENT CHARACTER");
    CHECK("\xF4\x8F\xBF\xBF", 4, utf8_error, "U+0010FFFF <not a character>");
    CHECK("\xF4\x90\x80\x80", 4, utf8_error, "U+00110000 <not a character>");

    return 0;
}
