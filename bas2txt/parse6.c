/****************************************************/
/* bas2txt                                          */
/* parse6.c                                         */
/* 2001.06.27. by ishioka                           */
/* 2003.11.01. by ishioka                           */
/* 2012.04.25. by fuzzball                          */
/****************************************************/

#include <math.h>
#include "bas2txt.h"

#define WORDLEN		8

// N66: 83 / N66SR : 84
#define CODEDATA	(0x84)

// N66: 8e / N66SR : 8f
#define CODEREM		(0x8f)

// N66SR : E9
#define CODEQUOTE	(0xe9)

// N66SR : 9F
#define CODEELSE	(0x9f)

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

// p6 code list
#include "p6code6.h"


// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------



// ------------------------------------------------------------
// function definition
// ------------------------------------------------------------

// ------------------------------------------------------------
// parsemain6 : parse line for mode 6
int parsemain6(void)
{
	int c, i, j, k;
	double val;

	for (;;) {
		c = buf_fgetc();
		if (c == 0) {
			// line end
			buf_fputc('\n');
			break;
		} else if (c == 0x0a) {
			// line feed
			buf_fputc('\n');
		} else if (c == 0x0b) {
			// octet
			buf_fprintf("&O");
			c = buf_fgetc();
			c += buf_fgetc() << 8;
			buf_fprintfo(c);
		} else if (c == 0x0c) {
			// hex
			buf_fprintf("&H");
			c = buf_fgetc();
			c += buf_fgetc() << 8;
			buf_fprintfx(c);
		} else if (c == 0x0d) {
			// jump address
			c = buf_fgetc();
			c += buf_fgetc() << 8;
			buf_fprintfd(c);
		} else if ((c == 0x0e) || (c == 0x1c)) {
			// 0e xx xx : line number
			// 1c xx xx : integer
			c = buf_fgetc();
			c += buf_fgetc() << 8;
			buf_fprintfd(c);
		} else if (c == 0x0f) {
			// 0f xx : integer (10..255)
			c = buf_fgetc();
			buf_fprintfd(c);
		} else if ((c >= 0x11) && (c <= 0x1b)) {
			// 11..1b : 0 .. 10
			buf_fprintfd(c - 0x11);
		} else if (c == 0x1d) {
			// 1d xx xx xx xx xx : real number
			val = 0.0;
			for (j = 0; j < 4; j++) {
				c = buf_fgetc();
			 	for (k = 0; k < 8; k++) {
					if (c & (1 << k)) {
			 			val += 1.0;
					}
					val /= 2.0;
				}
			}
			val *= 2.0;
			val = (val + 1) / 2.0;
			c = buf_fgetc();
			if (c == 0x80) {
				val = 0.0;
			} else {
                val = val * pow(2.0, (c - 0x80));
			}
			buf_fprintfG(val);
		} else {
			// check this is a function (which code is ff xx)
			i = (c == 0xff) ? 1 : 0;
			if (c == 0xff) {
				c = buf_fgetc();
			}
			if ((c >= 0x80) && (p6codelist6[i][c - 0x80][0] != '\0')) {
				// if match, output command strings
				buf_fprintf(p6codelist6[i][c - 0x80]);
				if ((i == 0) && (c == CODEDATA)) {
					// raw output until return or colon
					if (parse_data() == 0x00) {
						return(0);
					}
				} else if ((i == 0) &&
						   ((c == CODEREM) || (c == CODEQUOTE))) {
					// raw output until return
					parse_rem();
					return(0);
				}
			} else {
				// if not code, output 1 byte as is
				if (i == 1) {
					buf_fputc(0xff);
				}
				if ((c == ':') && (*(inptr) == CODEREM) && (*(inptr + 1) == CODEQUOTE)) {
					inptr++;
					// output nothing
                } else if ((c == ':') && (*(inptr) == CODEELSE)) {
                    // output nothing
                } else {
					buf_fputc(c);
					if (c == '"') {
						// raw output until " or return
						if (parse_quote() == 0x00) {
							return(0);
						};
					}
				}
			}
		}
	}
	return(0);
}
