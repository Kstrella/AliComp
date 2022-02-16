/*
	You can use these two print statements for the errors:
		printf("Virtual Machine Error: Stack Overflow Error\n");
		printf("Virtual Machine Error: Out of Bounds Access Error\n");
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define REG_FILE_SIZE 10
#define MAX_STACK_LENGTH 100

void print_execution(int line, char *opname, instruction IR, int PC, int BP, int SP, int *stack, int *RF)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.r, IR.l, IR.m, PC, SP, BP);

	// print register file
	for (i = 0; i < REG_FILE_SIZE; i++)
		printf("%d ", RF[i]);
	printf("\n");

	// print stack
	printf("stack:\t");
	for (i = MAX_STACK_LENGTH - 1; i >= SP; i--)
		printf("%d ", stack[i]);
	printf("\n");
}

int base(int L, int BP, int *stack)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = stack[rtn];
		ctr--;
	}
	return rtn;
}

void execute_program(instruction *code, int printFlag)
{
	// keep this

	int BP = MAX_STACK_LENGTH -1;
	int SP = BP + 1;
	int PC = 0;
	int IR;

	if (printFlag)
	{
		printf("\t\t\t\t\tPC\tSP\tBP\n");
		printf("Initial values:\t\t\t\t%d\t%d\t%d\n", PC, SP, BP);


	}

	/*
	BP = MAX_STACK_LENGTH - 1 // Base Pointer – Points to the base of the
	current AR
	SP = BP + 1 // Stack Pointer – Points to the top entry of
	the stack
	PC = 0 // Program Counter – Points to next
	instruction
	IR // Instruction Register - holds the current
	instructioncan be an array or a struct
	*/
	switch(instruction){
		//LIT
		case 1:
			rf[instruction.r] = instruction.m;
			break;

			//RET
			case 2:

				break;
	}
}

//fetch(IS,PC);
instruction fetch(instruction *IS,int PC)
{
	return IS[PC];
}

}
