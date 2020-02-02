#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operations.h"

char *infile;
char *outfile;

int
main (int argc, char** argv)
{

	if (argc < 2) {
		puts("no input file provided");
		exit(-1);
	}

	/* use first arg as infile */
	infile = argv[1];
	
	/* use second arg as outfile or default to a.rom */
	if (argc > 2) {
		outfile = argv[2];
	} else {
		outfile = malloc(6);
		strcpy(outfile, "a.rom");
	}


	
	exit(0);
}
