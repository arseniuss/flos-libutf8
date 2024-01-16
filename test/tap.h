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

#ifndef TAP_H
#define TAP_H

#include <stdio.h>
#include <stdlib.h>

static int _tap_test = 0;

#define plan(N)       printf("1..%d\n", (N))
#define skip_all(S)   printf("1..0 # %s\n", (S))

#define pass(S)       (printf("ok %d - %s\n", ++_tap_test, (S)), 1)
#define fail(S)       (printf("not ok %d - %s\n", ++_tap_test, (S)), 0)

#define ok(P, S)      ((P) ? pass(S) : fail(S))
#define is(A, B, S)   ok((A) == (B), (S))
#define isnt(A, B, S) ok((A) != (B), (S))

#define skip(N, S)                                                                                                     \
    do {                                                                                                               \
        int _tap_skip = _tap_test + (N);                                                                               \
        while (_tap_test < _tap_skip)                                                                                  \
            printf("ok %d # SKIP %s\n", ++_tap_test, (S));                                                             \
    } while (0)

#define diag(S)     fprintf(stderr, "# %s\n", (S))
#define note(S)     fprintf(stdout, "# %s\n", (S))

#define bail_out(S) (printf("Bail out! %s\n", (S)), exit(0))

#endif
