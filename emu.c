
/*
   Written by depsterr in team enhance for the 2019 x-mas ctf emu challange
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define byte unsigned char

typedef struct registers{
	    byte A; // 8 bits
	    unsigned PC: 12; // 10 bits
}regs;

typedef union twelve_bit{
	unsigned addr: 12;
}twelve_bit;

byte step = 0;

int main(int argc, char** argv){
	
	FILE *writefp = fopen("rom_log", "w");

	float waittime = 0.25f;
	if(argc > 2)
		sscanf(argv[2], "%f", &waittime);
	if(argc > 3)
		step = 1;

	// Load program into memory
	char* instruction_read_buffer = NULL;
	unsigned int instruction_length;
	FILE* file_ptr = fopen (argv[1], "rb");

	if (!file_ptr) {
		printf("ROM does not exist!\n");
		return -1;
	} else {
		fseek(file_ptr, 0, SEEK_END);
		instruction_length = ftell(file_ptr);
		fseek(file_ptr, 0, SEEK_SET);
		instruction_read_buffer = malloc(instruction_length);
		if (instruction_read_buffer) {
			fread(instruction_read_buffer, 1, instruction_length, file_ptr);
		}
		fclose(file_ptr);
	}

	printf("rom size: 0x%X\n", instruction_length);

	byte instruction_buffer[0x1000];
	byte security_buffer[0x1000];
	memset(instruction_buffer, 0x0, 0x1000);
	memset(security_buffer, 0x1, 0x1000);

	printf("loading rom into memory\n");
	for(int n = 0; n < instruction_length; n++)
		instruction_buffer[0x100 + n] = instruction_read_buffer[n];

	regs registers;

	registers.A = 0x0; // Accumulator
	registers.PC = 0x100; // Program counter

	twelve_bit tmp;
	tmp.addr = 0x0;

	printf("engaging main loop\n");
	for(;;){
		if(step)
			getchar();
		else
			sleep(waittime);
		printf("current adress: 0x%00X, current value in A: 0x%00X, current instruction: 0x%00X:%00X\n", registers.PC, registers.A, instruction_buffer[registers.PC], instruction_buffer[registers.PC + 1]);

		if(instruction_buffer[registers.PC] == 0x00){

			registers.A += instruction_buffer[registers.PC + 1];
			printf("Added %00X to A!\n", instruction_buffer[registers.PC + 1]);
			registers.PC += 2;
			continue;

		}else if(instruction_buffer[registers.PC] == 0x01){

			registers.A = instruction_buffer[registers.PC + 1];
			printf("Set A to %00X\n", instruction_buffer[registers.PC + 1]);
			registers.PC += 2;
			continue;

		}else if(instruction_buffer[registers.PC] == 0x02){

			registers.A ^= instruction_buffer[registers.PC + 1];
			printf("xor:ed A with %00X\n", instruction_buffer[registers.PC + 1]);
			registers.PC += 2;
			continue;

		}else if(instruction_buffer[registers.PC] == 0x03){

			registers.A |= instruction_buffer[registers.PC + 1];
			printf("or:ed A with %00X\n", instruction_buffer[registers.PC + 1]);
			registers.PC += 2;
			continue;

		}else if(instruction_buffer[registers.PC] == 0x04){

			registers.A &= instruction_buffer[registers.PC + 1];
			printf("and:ed A with %00X\n", instruction_buffer[registers.PC + 1]);
			registers.PC += 2;
			continue;
			
		}else if(instruction_buffer[registers.PC] == 0x60){

			printf("Comparing A with %00X\n", instruction_buffer[registers.PC + 1]);
			if(registers.A > instruction_buffer[registers.PC + 1])
				registers.A = 255;
			else if(registers.A < instruction_buffer[registers.PC + 1])
				registers.A = 1;
			else if(registers.A == instruction_buffer[registers.PC + 1])
				registers.A = 0;
			registers.PC += 2;
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x08){
			
			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			printf("Setting value of A to value at adress %00X: %00X\n",tmp.addr, instruction_buffer[tmp.addr]);
			registers.A = instruction_buffer[tmp.addr];
			registers.PC += 2;
			continue;

		} else if((instruction_buffer[registers.PC] >> 4) == 0x0D){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			if(security_buffer[tmp.addr] == 1){
				printf("xor:ing %00X with A, value before xor %00X\n", tmp.addr, instruction_buffer[tmp.addr]);
				instruction_buffer[tmp.addr] ^= registers.A;
				printf(" value after xor %00X\n", instruction_buffer[tmp.addr]);
			}else
				printf("no xor. protected byte!\n");
			registers.PC += 2;
			continue;

		} else if((instruction_buffer[registers.PC] >> 4) == 0x0F){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			printf("writing value of A to adress %00X\n", tmp.addr);
			if(security_buffer[tmp.addr] == 1){
				instruction_buffer[tmp.addr] = registers.A;
			}else{
				printf("no write, protected byte!\n");
			}
			registers.PC += 2;
			continue;

		}else if((instruction_buffer[registers.PC] == 0x13) && (instruction_buffer[registers.PC + 1] == 0x37)){

			printf("print instruction for %c\n", registers.A);
			putchar(registers.A);
			putchar(0x0a);
			writefp = fopen("rom_log", "a");
			fputc(registers.A, writefp);
			fclose(writefp);
			if(getchar() == 's')
				step = 1;

			registers.PC += 2;
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x02){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			printf("unconditional jump, jumping to 0x%X\n", tmp.addr);
			registers.PC = tmp.addr;
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x03){

			if(registers.A == 0){
				tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
				printf("A = 0, jumping to 0x%X\n", tmp.addr);
				registers.PC = tmp.addr;
			}else{
				registers.PC += 2;
				printf("A != 0, no jump.\n");
			}
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x04){

			if(registers.A == 1){
				tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
				printf("A = 1, jumping to 0x%X\n", tmp.addr);
				registers.PC = tmp.addr;
			}else{
				registers.PC += 2;
				printf("A != 1, no jump.\n");
			}
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x05){

			if(registers.A == 255){
				tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
				printf("A = FF, jumping to 0x%X\n", tmp.addr);
				registers.PC = tmp.addr;
			}else{
				registers.PC += 2;
				printf("A != FF, no jump.\n");
			}
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x07){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			printf("Comparing A and adress 0x%00X, %00X == %00X\n", tmp.addr, registers.A, instruction_buffer[tmp.addr]);
			if(registers.A > instruction_buffer[tmp.addr])
				registers.A = 255;
			else if(registers.A < instruction_buffer[tmp.addr])
				registers.A = 1;
			else if(registers.A == instruction_buffer[tmp.addr])
				registers.A = 0;
			registers.PC += 2;
			continue;

		}else if((instruction_buffer[registers.PC] == 0xBE) &&  (instruction_buffer[registers.PC + 1] == 0xEF)){

			printf("BEEF, setting x to 0x100 and A to 0x42\n");
			registers.PC = 0x100;
			registers.A = 0x42;
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x09){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			printf("Blocking writes to %00X\n", tmp.addr);
			security_buffer[tmp.addr] = 0;
			registers.PC += 2;
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x0A){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			printf("Unblocking writes to %00X\n", tmp.addr);
			security_buffer[tmp.addr] = 1;
			registers.PC += 2;
			continue;
		}else if((instruction_buffer[registers.PC] == 0xEE) && (instruction_buffer[registers.PC + 1] == 0xEE)){
			printf("EEEE, skipping\n");
			registers.PC += 2;
			continue;

		}else if((instruction_buffer[registers.PC] >> 4) == 0x0C){

			tmp.addr = ((instruction_buffer[registers.PC] & 0x0f) << 8) + (instruction_buffer[registers.PC + 1]);
			if(security_buffer[tmp.addr] == 1){
				printf("xor:ing adress %00X with 0x42\n", tmp.addr);
				instruction_buffer[tmp.addr] ^= 0x42;
			}
			registers.PC += 2;
			continue;

		}else{

			printf("unknown instruction, decrementing A!\n");
			registers.PC += 2;
			registers.A--;
			continue;

		}
	}
}
