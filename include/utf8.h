/* See LICENSE file for copyright and license details. */
#ifndef UTF8_H
#define UTF8_H

#include <stddef.h>
#include <stdint.h>

#define UTF8_CHAR(x)    INT32_C(x)

#define utf8_max        6 /* maximum bytes per character */

#define utf8_single     0x80 /* if < then single character */
#define utf8_error      ((utf8_char)-1)

typedef int32_t utf8_char;

#ifdef __cplusplus
extern "C" {
#endif

    int utf8_isalpha(utf8_char c);
    int utf8_iscntrl(utf8_char c);
    int utf8_isdigit(utf8_char c);
    int utf8_toint(utf8_char c);
    int utf8_islower(utf8_char c);
    int utf8_isspace(utf8_char c);
    int utf8_istitle(utf8_char c);
    int utf8_isupper(utf8_char c);

    utf8_char utf8_tolower(utf8_char c);
    utf8_char utf8_toupper(utf8_char c);

    /**
     * Checks if UTF code point is valid character
     * 
     * Note. Old name `isvalidrune`
     */
    int utf8_isvalid(utf8_char r);

    /**
     * Returns number of UTF-8 code points in UTF-8 string `s`
     * 
     * Note. Old name `utflen`
     */
    size_t utf8_strlen(const char *s);

    /**
     * Returns number of UTF-8 code points represented by first `len` bytes of the UTF-8 string `s`.
     * If the final sequence is incomplete it will not be counted.
     * 
     * Note. Old name `utfnlen`
     */
    size_t utf8_strnlen(const char *s, size_t len);

    /**
     * Converts at most `len` bytes starting at `s` to one character at `p`, and returns the number of bytes copied.
     * If the next sequence is longer than `len` bytes then will return 0.
     * 
     * Note. Old name `charntorune`
     */
    int utf8_nstr2chr(utf8_char * p, const char * s, size_t len);

    /**
     * Converts at most `utf_max` bytes starting at `s` to one character at `p`, and returned the number of bytes
     * copied.
     * If the input is invalid UTF-8, will convert the sequence to `utf_error` (0xFFFD) and return the number of bytes 
     * in the invalid sequence.
     * 
     * Note. Old name `chartorune`.
     */
    int utf8_str2chr(utf8_char * p, const char * s);

    /**
     * Converts one character at `p` to at most `utf_max` bytes starting at `s`  and returns the number of bytes
     * copied.
     * The `utf_max` is the maximum of bytes required to represent UTF-8 character.
     * If the character is illegal, will return 0;
     * 
     * Note. Old name `runetochar`.
     */
    int utf8_chr2str(char *s, const utf8_char *r);

    /**
     * Returns the number of characters represented by the first `len` bytes of the UTF-8 string `s`.
     * If the final sequence is incomplete it will be not be counted.
     * 
     * Note. Old name `utfnlen`.
     */
    int utf8_nlen(const char *s, size_t len);

    /**
     * Returns number of bytes required to convert the UTF-8 character `r` into UTF-8.
     * If the character is illegal will return 0.
     * 
     * Note. Old name `runelen`.
     */
    int utf8_chrlen(utf8_char r);

    /**
     * Returns number of bytes required to convert `len` characters pointed by `r` info UTF-8.
     * 
     * Note. Old name `runenlen`.
     */
    size_t utf8_nchrlen(const utf8_char *r, size_t len);

    char *utf8_strchr(const char *s, utf8_char r);
    char *utf8_strrchr(const char *s, utf8_char r);
    char *utf8_strstr(const char *s1, const char *s2);


    extern const unsigned char utf8_tbl[64];


#ifdef __cplusplus
}
#endif

#endif
