/****************************************************/
/* bas2txt                                          */
/* buffer.c                                         */
/* 2001.06.27. by ishioka                           */
/* 2003.11.01. by ishioka                           */
/* 2012.04.25. by fuzzball                          */
/****************************************************/

#include "bas2txt.h"

#include <sys/stat.h>

#define BUFSIZE 1024

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

// sjis <-> p6 chatacter set table
#include "sjis.h"

unsigned char *inptr, *inptrbak;

static unsigned char *inptrend;

// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------


// ------------------------------------------------------------
// function definition
// ------------------------------------------------------------

// ------------------------------------------------------------
// buf_fgetc
int buf_fgetc(void)
{
	int c;
	if (inptr < inptrend) {
		c = *(inptr++);
	} else {
		c = EOF;
		b2t_exit("EOF reached");
	}
	return c;
}

// ------------------------------------------------------------
// buf_fread : like fread?
void buf_fread(const char *filename )
{
	struct stat st;
	size_t readsize;

	stat(filename, &st);
	inptr = (unsigned char *)malloc(st.st_size);
	if (inptr == NULL) {
		b2t_exit("Memory allocation error!");
	}
	readsize = fread((void *)inptr, 1, st.st_size, infp);
	if (readsize != st.st_size) {
		b2t_exit("File read error!");
	}
	inptrbak = inptr;
	inptrend = inptr + readsize;
}

// ------------------------------------------------------------
// buf_fputc : like fputc
void buf_fputc(int c)
{
	static int prev = 0;
	int wc;

	if (c == 0x14) {
		prev = 1;
	} else {
		if (prev == 1) {
			// for semi-graphic character
			c -= 0x30;
		}
		//if (c <= 0x1f) {
		if (prev == 0 && c == '\n') {
            //fputc(c, outfp);
            fputc('\r', outfp);
            fputc('\n', outfp);
        } else {
			if (useascii && (c >= ' ') && (c <= '~')) {
				fputc(c, outfp);
			} else {
				wc = sjistbl[c];
				fputc(wc >> 8, outfp);
				fputc(wc & 0xff, outfp);
			}
		}
		prev = 0;
	}
}

// ------------------------------------------------------------
// buf_fprintfd : like fprintf
void buf_fprintfd(int d)
{
	char str[BUFSIZE];
	sprintf(str, "%d", d);
	buf_fprintf(str);
}

// ------------------------------------------------------------
// buf_fprintfx : like fprintf
void buf_fprintfx(int x)
{
	char str[BUFSIZE];
	sprintf(str, "%X", x);
	buf_fprintf(str);
}

// ------------------------------------------------------------
// buf_fprintfo : like fprintf
void buf_fprintfo(int o)
{
	char str[BUFSIZE];
	sprintf(str, "%o", o);
	buf_fprintf(str);
}

// ------------------------------------------------------------
// buf_fprintfG : like fprintf
void buf_fprintfG(double g)
{
	char str[BUFSIZE];
	sprintf(str, "%.8G", g);
	buf_fprintf(str);
}

// ------------------------------------------------------------
// buf_fprintf : like fprintf
void buf_fprintf(char *s)
{
	int len, i;

	len = strlen(s);
	for (i = 0; i < len; i++) {
		buf_fputc(s[i]);
	}
}
