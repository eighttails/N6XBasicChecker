/****************************************************/
/* bas2txt                                          */
/* main.h                                           */
/* 2001.06.27. by ishioka                           */
/* 2003.11.01. by ishioka                           */
/* 2012.04.25. by fuzzball                          */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

extern FILE *infp, *outfp;
extern unsigned char *inptr, *inptrbak;
extern int useascii;


// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------

// main.c
void b2t_exit(char *msg);
unsigned char parse_data(void);
void parse_rem(void);
unsigned char parse_quote(void);

// parse.c
int parsemain5(void);
int parsemain6(void);

// buffer.c
int buf_fgetc(void);
void buf_fread(const char *filename);
void buf_fputc(int c);
void buf_fprintfd(int d);
void buf_fprintfx(int x);
void buf_fprintfo(int o);
void buf_fprintfG(double g);
void buf_fprintf(char *s);
