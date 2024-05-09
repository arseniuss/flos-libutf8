#!env make

#
# flos/libutf8
#
# © 2012-2016 Connor Lane Smith <cls@lubutu.com>
# © 2015 Laslo Hunhold <dev@frign.de>
# © 2022-2024 Armands Arseniuss Skolmeisters <arseniuss@arseniuss.id.lv>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

.POSIX:

LIB = lib/libutf8.a
HDR = include/flos/utf8.h

GEN = \
    source/isalpha.c \
    source/iscntrl.c \
    source/isdigit.c \
    source/islower.c \
    source/isspace.c \
    source/istitle.c \
    source/isupper.c

GENOBJ = $(GEN:.c=.o)

SRC = \
    source/chr2str.c \
    source/chrlen.c \
    source/isvalid.c \
    source/nlen.c \
    source/str2chr.c \
    source/strchr.c \
    source/strlen.c \
    source/strrchr.c \
    source/strstr.c \
    source/type.c \
    $(GEN)

OBJ = $(SRC:.c=.o)

TESTSRC = \
    test/boundary.c \
    test/kosme.c \
    test/malformed.c \
    test/overlong.c

TEST = $(TESTSRC:.c=)

CFLAGS += -D_POSIX_C_SOURCE=200809L
CFLAGS += -D_XOPEN_SOURCE=700

include config.mk

.PHONY: all check clean

all: $(LIB)

$(LIB): $(OBJ)
	mkdir -p lib
	rm -f $@
	$(AR) -rcs $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIB)

$(GEN): bin/build_c.awk share/UnicodeData.txt source/type.h source/typebody.h
	$(AWK) -f bin/build_c.awk share/UnicodeData.txt

$(GENOBJ) source/type.o: source/type.c

$(TEST): $(LIB) test/tap.h

check: $(TEST)
	prove $(TEST)

clean:
	rm -vf $(GEN) $(OBJ) $(LIB) $(TEST)
