#include <stdio.h>
#include "txt2bas.h"


// ------------------------------------------------------------
// main : txt2bas main routine
int main(int argc, char **argv)
{
    int srmode;

    // check arguments
    if (argc == 5) {
        if (strcmp(argv[1], "-6") == 0) {
            srmode = 1;
        } else if (strcmp(argv[1], "-5") == 0) {
            srmode = 0;
        } else {
            usage();
        }
    } else if (argc == 4) {
        srmode = 0;
    } else {
        usage();
    }

    // open files
    if ((infp = fopen(argv[argc - 3], "r")) == NULL) {
        fprintf(stderr, "Can't open input file %s\n", argv[argc - 2]);
        exit(1);
    }
    if ((outfp = fopen(argv[argc - 2], "wb")) == NULL) {
        fprintf(stderr, "Can't open output file %s\n", argv[argc - 1]);
        fclose(infp);
        exit(1);
    }

    char fname[7] = {};
    strncpy(fname, argv[argc - 1], 6);

    // main process
    mk_head(fname);
    for (;;) {
        if (buf_fgets() != 0)
            break;
        if (getlinenumber(srmode) <= 0) continue;
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

