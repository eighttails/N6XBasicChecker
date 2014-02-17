/****************************************************/
/* bas2txt                                          */
/* parse5.c                                         */
/* 2001.06.27. by ishioka                           */
/* 2012.04.25. by fuzzball                          */
/****************************************************/

#include "bas2txt.h"

#define WORDLEN		8

// N66: 83 / N66SR : 84
#define CODEDATA	(0x83)

// N66: 8e / N66SR : 8f
#define CODEREM		(0x8e)

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

// p6 code list
#include "p6code5.h"


// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------



// ------------------------------------------------------------
// function definition
// ------------------------------------------------------------

// ------------------------------------------------------------
// parsemain5 : parse line
int parsemain5(void)
{
	unsigned char c;

	for (;;) {
		c = buf_fgetc();
		if (c == 0) {
			// line end
			buf_fputc('\n');
			break;
		} else {
			if ((c >= 0x80) && (c <= 0xf9)) {
				// if match, output command strings
				buf_fprintf(p6codelist5[c - 0x80]);
				if (c == CODEDATA) {
					// raw output until return or colon
					if (parse_data() == 0x00) {
						return(0);
					};
				} else if (c == CODEREM) {
					// raw output until return
					parse_rem();
					return(0);
				}
			} else {
				// if not code, output 1 byte as is
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
	return(0);
}
