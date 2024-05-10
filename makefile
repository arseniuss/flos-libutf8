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

include config.mk

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

SRCS = \
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

TESTSRC = \
    test/boundary.c \
    test/kosme.c \
    test/malformed.c \
    test/overlong.c

TEST = $(TESTSRC:.c=)

CFLAGS += -Iinclude
CFLAGS += -D_POSIX_C_SOURCE=200809L
CFLAGS += -D_XOPEN_SOURCE=700

TEMPDIR != mktemp -d

OBJS != echo $(SRCS:.c=.o) | sed -e 's/source\//$(builddir)\/source\//g'
DEPS != echo $(SRCS:.c=.d) | sed -e 's/source\//$(builddir)\/source\//g'
PPS != echo $(SRCS:.c=.c.pp) | sed -e 's/source\//$(builddir)\/source\//g'

.SUFFIXES:
.PHONY: all check install install-strip distclean clean pkg

all: $(LIB)

$(LIB): $(OBJS)
	mkdir -p lib
	rm -f $@
	$(AR) -rcs $@ $(OBJS)

$(builddir)/%.o: %.c
	@mkdir -p $(@D)
	$(PP) $(CFLAGS) $< > $(builddir)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(builddir)/$*.d -c -o $@ $<

$(GEN): bin/build_c.awk share/UnicodeData.txt source/type.h source/typebody.h
	$(AWK) -f bin/build_c.awk share/UnicodeData.txt

$(GENOBJ) source/type.o: source/type.c

$(TEST): $(LIB) test/tap.h

check: $(TEST)
	prove $(TEST)

install: $(HDR) $(LIB)
	install -d $(DESTDIR)$(includedir)/
	install -m 644 $(HDR) $(DESTDIR)$(includedir)/
	install -d $(DESTDIR)$(libdir)
	install -m 644 $(LIB) $(DESTDIR)$(libdir)

install-strip: $(HDR) $(LIB)
	install -s -m 644 $(LIB) $(DESTDIR)$(libdir)/

dist:
	rm -rf tmp

distclean: clean
	rm -vf config.mk UCD.zip share/UnicodeData.txt

pkg: all
	make DESTDIR=$(TEMPDIR) install
	cd $(TEMPDIR) && find . -type f -print > ../pkg.lst
	cd $(TEMPDIR) && tar zcvf $(srcdir)/$(PACKAGE)-$(VERSION)-$(TRIPLE).tar.gz `cat ../pkg.lst`
	rm -rf $(TEMPDIR) pkg.lst

clean:
	rm -vf $(GEN) $(OBJ) $(LIB) $(TEST)
