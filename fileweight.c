/*
 * How much does a file weigh?
 *
 * Based on IBM's research, one bit takes between
 * 1 and 62.5 million iron atoms on a normal hard
 * disk:
 * http://gizmodo.com/5875674/ibm-figures-out-how-many-atoms-it-takes-to-hold-a-bit-hint-its-12
 *
 * This application tries to estimate a medium
 * weight for the input file, based on a value of
 * (62.5+1)/2 = 31.75 million iron atoms per bit.
 *
 * Known shortcomings:
 *  - Currently, only platforms with a byte length
 *    of 8 are supported. Sorry, DEC!
 *
 * Usage:
 *   cc -lm -o fileweight fileweight.c
 *   fileweight some-file
 *
 * Licensed under the terms of the WTFPL v2.
 * http://wtfpl.net/txt/copying
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/param.h>

#define atoms_per_bit      31750000
#define mg_per_iron_atom   9.2732796 * exp(-20)

void show_syntax(char *argv[]) {
    /* Someone made a mistake. */
    printf("Invalid number of parameters or invalid file parameter specified!\n\n");
    printf("Usage:\n");
    printf("   %s <filename>\n\n", argv[0]);
}

int main(int argc, char *argv[]) {
#ifdef __OpenBSD__
    if (-1 == pledge("stdio rpath", NULL)) {
        /* Don't waste priviledges. */
        err(EXIT_FAILURE, "pledge");
    }
#endif

    if (argc != 2) {
        show_syntax(argv);
        return(EXIT_FAILURE);
    }

    char inputfile[MAXPATHLEN];
    off_t file_bytes;

    strlcpy(inputfile, argv[1], sizeof(inputfile));
    int fd_inputfile = open(inputfile, O_RDONLY);

    if (!fd_inputfile) {
        /* Couldn't open the file. */
        show_syntax(argv);
        return(EXIT_FAILURE);
    }

    file_bytes = lseek(fd_inputfile, 0, SEEK_END);
    close(fd_inputfile);

    long double weight = file_bytes * mg_per_iron_atom * atoms_per_bit * 1000 * 8;
    printf("The file %s weighs about %e grams.\n\n", argv[1], weight);

    return(EXIT_SUCCESS);
}
