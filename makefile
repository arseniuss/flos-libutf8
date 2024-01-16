#!env make
.POSIX:

LIB = lib/libutf8.a
HDR = include/utf8.h

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

tests: $(TEST)
	prove $(TEST)

clean:
	@ rm -f $(GEN) $(OBJ) $(LIB) $(TEST)
