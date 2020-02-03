#define byte unsigned char
#define word unsigned short
#define NOARG (Arg)0
#define ARGLEN(op) (4 - op.len)
#define HASARG(op) ARGLEN(op)

/* Line struct */
typedef struct Line {
	char str[64];
	byte words;
	byte islabel;
} Line;

/* Label struct */
typedef struct Label {
	char name[64];
	unsigned ptr : 12; 
} Label;
	
/* Union for args needed in Operation */
typedef union Arg {
	byte dnibble; /* Double nibble argument */
	unsigned tnibble : 12; /* Tripple nibble argument */
} Arg;

/* Struct to use for each operation */
typedef struct Operation {
	byte len; /* length in nibbles */
	short opcode; /* the opcode */
	Arg argument; /* the argument here */
} Operation;

/* Template operations that will be used in the program by intializing to these and changing args */

/* Definitions beginning with P use pointers */

/* Arithmetic */
#define ADD		{ .len = 2, .opcode = 0x00   }
#define LDA		{ .len = 2, .opcode = 0x01   }
#define XOR		{ .len = 2, .opcode = 0x02   }
#define OR		{ .len = 2, .opcode = 0x03   }
#define AND		{ .len = 2, .opcode = 0x04   }
#define PLDA	{ .len = 1, .opcode = 0x8    }
#define PXOR	{ .len = 1, .opcode = 0xD    }
#define PLD		{ .len = 1, .opcode = 0xF    }

/* I/O */
#define SOUT	{ .len = 4, .opcode = 0x1337,  .argument = NOARG }

/* Control Flow */
#define JMP		{ .len = 1, .opcode = 0x2    }
#define JE		{ .len = 1, .opcode = 0x3    }
#define JL		{ .len = 1, .opcode = 0x4    }
#define JG		{ .len = 1, .opcode = 0x5    }
#define CMP		{ .len = 2, .opcode = 0x60   }
#define PCMP	{ .len = 1, .opcode = 0x7    }
#define BEEF	{ .len = 4, .opcode = 0xBEEF }

/* Security */
#define PBLOCK	{ .len = 1, .opcode = 0x9    }
#define PUBLOCK	{ .len = 1, .opcode = 0xA    }
#define PFROB	{ .len = 1, .opcode = 0xC    }

/* Misc */
#define NOP		{ .len = 4, .opcode = 0xEEEE,  .argument = NOARG }
