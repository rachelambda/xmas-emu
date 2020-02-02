#define byte unsigned char
#define word unsigned short
#define NOARG (Arg)0
#define ARGLEN(op) (4 - op.len)

/* Union for args needed in Operation */
typedef union Arg {
	byte dnibble; /* double nibble argument */
	unsigned tnibble : 12; /* tripple nibble argument */
} Arg;

/* Struct to use for each operation */
typedef struct Operation {
	byte len; /* length in nibbles */
	short opcode; /* the opcode */
	byte hasargument; /* does it take arguments? */
	Arg argument; /* the argument here */
} Operation;

/* Template operations that will be used in the program by intializing to these and changing args */

#define ADD		{ .len = 2, .opcode = 0x00,   .hasargument = 1 }
#define LDA		{ .len = 2, .opcode = 0x01,   .hasargument = 1 }
#define XOR		{ .len = 2, .opcode = 0x02,   .hasargument = 1 }
#define OR		{ .len = 2, .opcode = 0x03,   .hasargument = 1 }
#define AND		{ .len = 2, .opcode = 0x04,   .hasargument = 1 }
#define NOP		{ .len = 4, .opcode = 0xEEEE, .hasargument = 0, .argument = NOARG }
#define SOUT	{ .len = 4, .opcode = 0x1337, .hasargument = 0, .argument = NOARG }
