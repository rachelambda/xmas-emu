#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

/* File names */
char *infile;
char *outfile;

/* Operation buffer */
Operation *opbuffer = NULL;
unsigned short opsize = 0;
unsigned short oplen = 0;

/* Label buffer */
Label *lbbuffer = NULL;
unsigned int lbsize = 0;
unsigned int lblen = 0;

/* Rom to write to file */
byte romb[0xEFF];

int
addoperation(Operation op)
{
	oplen++;
	opsize += sizeof(Operation);
		if (oplen > 0xEFF)
			puts("program instructions too large for memory, aborting");
			exit(-1);
		}
	if (realloc(opbuffer, opsize))
		return 1;
	else
		return 0;
}

int
addlabel(Label lb)
{
	lblen++;
	lbsize += sizeof(Operation);
	if (realloc(lbbuffer, lbsize))
		return 1;
	else
		return 0;
}

int
main (int argc, char **argv)
{

	/* Make sure an input file was provided */
	if (argc < 2) {
		puts("no input file provided");
		exit(-1);
	}

	/* Use first arg as infile */
	infile = argv[1];
	
	/* Use second arg as outfile or default to a.rom */
	if (argc > 2) {
		outfile = argv[2];
	} else {
		outfile = malloc(6);
		strcpy(outfile, "a.rom");
	}

	/* Load input file into memory */
	FILE* infp = fopen(infile, "rb");
	char* inb = NULL;
	unsigned int inl;

	if (!infp) {
		puts("unable to open file");
		exit(-1);
	} else {
		fseek(infp, 0, SEEK_END);
		inl = ftell(infp);
		fseek(infp, 0, SEEK_SET);
		inb = malloc(inl);
		if (inb) {
			fread(inb, 1, inl, infp);
		} else {
			puts("unable to malloc file buffer");
			exit(-1);
		}
		fclose(infp);
	}

	/* Read and interpret the input buffer */
	Line line;
	int lines;
	byte c = 0;
	for(lines = 0;; lines++) {
		/* Read one line */

		line = NOLINE;
		/* Check if it's a label by checking for whitespace */
		if ((inb[c] == ' ' || inb[c] == '\t') && inb[c - 1] == '\n')
			line.islabel = 0;
		else
			line.islabel = 1;
		/* Used for removing whitespace */
		byte finwh = 0;
		byte index = 0;
		for (;inb[c] != '\n'; c++){
			/* Filter whitespace */
			if (!((inb[c] == ' ' || inb[c] == '\t') && inb[c + 1] == (' ' || inb[c + 1] == '\t'))) {
				/* Copy over the string */
				line.str[i] = inb[c];
				index++;
				finwh = 0;
			} else if (finwh = 0) {
				finwh = 1;
				line.words++;
			}
		}

		/* Interpret line */

		/* Check if there are no more lines */
	}

	/* Exit with sucess code */
	exit(0);
}
