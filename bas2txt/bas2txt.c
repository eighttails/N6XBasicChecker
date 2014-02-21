/****************************************************/
/* bas2txt                                          */
/* main.c                                           */
/* 2001.06.27. by ishioka                           */
/* 2003.11.01. by ishioka                           */
/* 2012.04.25. by fuzzball                          */
/****************************************************/

#include "bas2txt.h"

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------

FILE *infp, *outfp;	// input/output file pointer
int useascii;		// output ascii character directly
static int line;	// current line number


// ------------------------------------------------------------
// function prototype
// ------------------------------------------------------------

void rd_head(void);
void usage(void);


// ------------------------------------------------------------
// function definition
// ------------------------------------------------------------

// ------------------------------------------------------------
// main : txt2bas main routine
int main(int argc, char **argv)
{
	int linkp;
	int srmode;
	int i;

	inptr = inptrbak = NULL;

	// check arguments
	srmode = 0;
	useascii = 0;
	if (argc < 3) {
		usage();
	}
	for (i = 1; i < argc - 2; i++) {
		if (argv[i][0] != '-') {
			usage();
		}
		if (argv[i][2] != '\0') {
			usage();
		}
		switch (argv[i][1]) {
			case '5':
				srmode = 0;
				break;
			case '6':
				srmode = 1;
				break;
			case 'a':
				useascii = 1;
				break;
			default:
				usage();
		}
	}

	// open files
	if ((infp = fopen(argv[argc - 2], "rb")) == NULL) {
		fprintf(stderr, "Can't open input file %s\n", argv[argc - 2]);
		exit(1);
	}
	if ((outfp = fopen(argv[argc - 1], "w")) == NULL) {
		fprintf(stderr, "Can't open output file %s\n", argv[argc - 1]);
		fclose(infp);
		exit(1);
	}

	// main process
	buf_fread(argv[argc - 2]);

	rd_head();
	for (;;) {
		// link pointer
		linkp = buf_fgetc();
		linkp += buf_fgetc() << 8;
		if (linkp == 0) {
			break;
		};
		
		// line number
		line = buf_fgetc();
		line += buf_fgetc() << 8;
		buf_fprintfd(line);
		buf_fputc(' ');

		if (srmode == 0) {
			parsemain5();
		} else {
			parsemain6();
		}
	}

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
    fprintf(stderr, "bas2txt version 0.3 : usage\n");
	fprintf(stderr, "  bas2txt [-56] [-a] infile outfile\n");
	exit(1);
}

// ------------------------------------------------------------
// b2t_exit : if error, output message, close files and exit
void b2t_exit(char *msg)
{
	fprintf(stderr, "%s %d\n", msg, line);
	fclose(infp);
	fclose(outfp);
	if (inptrbak != NULL) {
		free(inptrbak);
	}
	exit(1);
}

// ------------------------------------------------------------
// rd_head : read tape image header (0xd3 x 10 + filename)
void rd_head(void)
{
	int i = 0, c;

	line = 0;
	do {
		c = buf_fgetc();
		if (c == 0xd3) {
			i++;
		} else {
			i = 0;
		}
	} while (i < 10);

	buf_fprintf("Filename: ");

	for (i = 0; i < 6; i++) {
		c = buf_fgetc();
		if (c == 0x00) {
			c = ' ';
		}
		buf_fputc(c);
	}

	buf_fputc('\n');
	buf_fputc('\n');
}

// ------------------------------------------------------------
// parse_data : 
unsigned char parse_data(void)
{
	// raw output until return or colon
	int quoteflag;
	unsigned char c;

	c = buf_fgetc();
	quoteflag = 0;
	for (;;) {
		if (c == 0x00) {
			buf_fputc('\n');
			return(c);
		}
		buf_fputc(c);
		if ((c == ':') && (quoteflag == 0)) {
			return(c);
		} else if (c == '"') {
			quoteflag = 1 - quoteflag;
		}
		c = buf_fgetc();
	}
}

// ------------------------------------------------------------
// parse_rem : 
void parse_rem(void)
{
	unsigned char c;

	// raw output until return
	c = buf_fgetc();
	while (c != 0x00) {
		buf_fputc(c);
		c = buf_fgetc();
	}
	buf_fputc('\n');
	return;
}

// ------------------------------------------------------------
// parse_quote : 
unsigned char parse_quote(void)
{
	unsigned char c;

	// raw output until " or return
	c = buf_fgetc();
	for (;;) {
		if (c == 0x00) {
			buf_fputc('\n');
			return(c);
		}
		buf_fputc(c);
		if (c == '"') {
			return(c);
		}
		c = buf_fgetc();
	}
}
