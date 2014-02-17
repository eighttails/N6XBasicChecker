/****************************************************/
/* txt2bas                                          */
/* main.h                                           */
/* 2000.03.26. by ishioka                           */
/* 2014.02.17. by eighttails                        */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

extern FILE *infp, *outfp;


// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------
int txt2bas_main(int , char *, char* );

// main.c
void t2b_exit(char *msg);

// parse.c
int parsemain5(void);
int parsemain6(void);

// buffer.c
int buf_fgets(void);
char buf_fgetc(void);
void buf_ungetc(int n);
void buf_match(void);
void buf_nomatch(void);
void buf_progid(int n);

void usage(void);
int getlinenumber(int srmode);
void mk_head(void);
void mk_tail(void);
