#ifndef _MYSTRING_H_
#define _MYSTRING_H_

#include <ctype.h>
#include <string.h>

/* Some character defines */
#define NUL	0x00
#define TAB	0x09
#define LF	0x0A
#define CR	0x0D
#define SPACE	0x20

char *add_str(const char *s1, const char *s2);
int add_fmt_str(char *s, const char *format, ...);
size_t chr_idx(const char *s, char c);
size_t last_chr_idx(const char *s, char c);
char *to_lower_str(char *s);
char *to_upper_str(char *s);
size_t to_print_str(char *s, char c);
void xstr_to_bytes(const char *s, size_t count, unsigned char *buf);
void bytes_to_xstr(const unsigned char *buf, size_t count, char *s);
int set_strlen(char *s, size_t max);
char *term_str(char *s, int(*callback)(const char *));
int trim_str(char *s);

int _isascii(int c);
int is_empty_str(const char *s);
int is_empty_substr(const char *s, size_t count);
int is_dec_str(const char *s);
int is_hex_str(const char *s);
int is_print_str(const char *s);

#endif /*_MYSTRING_H_*/
