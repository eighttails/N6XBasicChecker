/****************************************************/
/* txt2bas                                          */
/* main.c                                           */
/* 2000.03.26. by ishioka                           */
/* 2003.05.05. by ishioka                           */
/* 2012.04.28. by fuzzball                          */
/****************************************************/

#include "txt2bas.h"

#define	_VER	"modified 002"

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

FILE *infp, *outfp;	// input/output file pointer
static int line;	// current line number


// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------

void usage(void);
int getlinenumber(void);
void mk_head(void);
void mk_tail(void);


// ------------------------------------------------------------
// function definition
// ------------------------------------------------------------


// ------------------------------------------------------------
// main : txt2bas main routine
int txt2bas_main(int srmode, char *infile, char* outfile)
{
	// open files
    if ((infp = fopen(infile, "r")) == NULL) {
        fprintf(stderr, "Can't open input file %s\n", infile);
		exit(1);
	}
    if ((outfp = fopen(outfile, "wb")) == NULL) {
        fprintf(stderr, "Can't open output file %s\n", outfile);
		fclose(infp);
		exit(1);
	}

	// main process
	mk_head();
	for (;;) {
		if (buf_fgets() != 0)
			break;
		if (getlinenumber() <= 0) continue;
		if (srmode == 0) {
			parsemain5();
		} else {
			parsemain6();
		};
	}
	mk_tail();

	// close files
	fclose(infp);
	fclose(outfp);

	// exit
	return(0);
}


// ------------------------------------------------------------
// usage : 
void usage(void)
{
	fprintf(stderr, "txt2bas version 0.7 [%s] : usage\n", _VER);
	fprintf(stderr, "  txt2bas [-56] infile outfile\n");
	exit(1);
}

// ------------------------------------------------------------
// getlinenumber : get and output line number
int getlinenumber(void)
{
	int c;
	static int prevline = -1;

	line = 0;

	// skip cr/tab/space
	do {
		c = buf_fgetc();
	} while (/*c == '\t' ||*/ c == ' ');
    if (c == '\r' || c == '\n') return 0;//skip line
	while (isdigit(c) != 0) {
		line = line * 10 + c - '0';
		c = buf_fgetc();
	}
	// skip cr/tab/space
	while (/*c == '\t' ||*/ c == ' ')
		c = buf_fgetc();
    if (c == '\r' || c == '\n') return -1;//skip line (?UL Error)
	buf_ungetc(1);
	if (prevline >= line)
		t2b_exit("Illegal line number found in");
	prevline = line;

	buf_match();
	// link pointer
	fputc(0xff, outfp);
	fputc(0xff, outfp);
	// line number
	fputc(line & 0x00ff, outfp);
	fputc((line & 0xff00) >> 8, outfp);

	return line;// > 0 : true
}

// ------------------------------------------------------------
// t2b_exit : if error, output message, close files and exit
void t2b_exit(char *msg)
{
	fprintf(stderr, "%s %d\n", msg, line);
	fclose(infp);
	fclose(outfp);
	exit(1);
}

// ------------------------------------------------------------
// mk_head : make tape image header
void mk_head(void)
{
	int i;

	for (i = 0; i < 10; i++)
		fputc(0xd3, outfp);
	// filename is "t2b"
	fprintf(outfp, "t2b");
	for (i = 0; i < 3; i++)
		fputc(0x00, outfp);
}

// ------------------------------------------------------------
// mk_tail : make tape image tail
void mk_tail(void)
{
	int i;

	for (i = 0; i < 2+9; i++)
		fputc(0x00, outfp);
}
