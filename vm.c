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
	// Base Pointer – Points to the base of the	current activation record
	int BP = MAX_STACK_LENGTH - 1;
	// Stack pointer - Points to the top entry of the stack
	int SP = BP + 1;
	// Program counter - Points to the next instruction
	int PC = 0;
	// Instruction Register - holds the current instruction can be an array or struct
	instruction IR;

	if (printFlag)
	{
		printf("\t\t\t\t\tPC\tSP\tBP\n");
		printf("Initial values:\t\t\t\t%d\t%d\t%d\n", PC, SP, BP);
	}

	int halt = false;
	
	// TODO
	// Need to loop over *code and nest the below switch into that loop construct

	switch (IR)
	{
	// LIT
	case 1:
		RF[IR.r] = IR.m;
		break;
	// RET
	case 2:
		// Return from current procedure (X) to the last procedure (Y).
		// SP = the index of the bottom of Y’s AR
		// BP = dynamic link value from X’s AR
		// PC = return address value from X’s AR
		break;
	// LOD
	case 3:
		if (base(IR.l) - IR.m < 0 || base(IR.l) - IR.m >= MAX_STACK_LENGTH)
		{
			// OUT OF BOUNDS ERROR
			// print error
			halt = true;
			// print trace
			break;
		}
		// Load value to register IR.r from the stack location at offset RF[IR.m] from L levels up
		RF[IR.r] = stack[base(IR.l) - RF[IR.m]];
		break;
	// STO
	case 4:
		if (base(IR.l) - IR.m < 0 || base(IR.l) - IR.m >= MAX_STACK_LENGTH)
		{
			// OUT OF BOUNDS ERROR
			// print error
			halt = true;
			// print trace
			break;
		}
		stack[base(IR.l) - RF[IR.m]] = RF[IR.r];
		break;
	// CAL
	case 5:
		// Call procedure at code index M.
		// 3 values in the AR:
		// 		1st - static link = base(L)
		// 		2nd - dynamic link = BP
		// 		3rd - return address = PC
		// After creating the AR:
		// 		BP = the index of the first entry of the new AR
		// 		PC = IR.m
		break;
	// INC
	case 6:
		SP -= IR.m;
		if (SP < 0)
		{
			// STACK OVERFLOW ERROR
			// print error
			halt = true;
			// print trace
		}
		break;
	// JMP
	case 7:
		// TODO
		// JUMP TO INSTRUCTION M
		break;
	// JPC
	case 8:
		if (RF[IR.r] == 0)
		{
			// TODO
			// JUMP TO INSTRUCTION AT IR.m
		}
		break;
	// WRT
	case 9:
		printf(RF[IR.r]);
		break;
	// RED
	case 10:
		RF[R] = scanf();
		break;
	// HAL
	case 11:
		halt = true;
		break;
	// NEG
	case 12:
		RF[IR.r] = ~RF[IR.r];
		break;
	// ADD
	case 13:
		RF[IR.r] = RF[IR.l] + RF[IR.m];
		break;
	// SUB
	case 14:
		RF[IR.r] = RF[IR.l] - RF[IR.m];
		break;
	// MUL
	case 15:
		RF[IR.r] = RF[IR.l] * RF[IR.m];
		break;
	// DIV
	case 16:		
		RF[IR.r] = RF[IR.l] / RF[IR.m];
		break;
	// MOD
	case 17:
		RF[IR.r] = RF[IR.l] % RF[IR.m];
		break;
	// EQL
	case 18:
		RF[IR.r] = (RF[IR.l] == RF[IR.m]) ? 1 : 0;
		break;
	// NEQ
	case 19:
		RF[IR.r] = (RF[IR.l] != RF[IR.m]) ? 1 : 0;
		break;
	// LSS
	case 20:
		RF[IR.r] = (RF[IR.l] < RF[IR.m]) ? 1 : 0;
		break;
	// LEQ
	case 21:
		RF[IR.r] = (RF[IR.l] <= RRF[IR.m]) ? 1 : 0;
		break;
	// GTR
	case 22:
		RF[IR.r] = (RF[IR.l] > RF[IR.m]) ? 1 : 0;
		break;
	// GEQ
	case 23:
		RF[IR.r] = (RF[IR.l] >= RF[IR.m]) ? 1 : 0;
		break;
	case default:
		break;
	}
}

// Returns: instruction as defined in 'compiler.h'
// example call:
// 		fetch(IS, PC);
instruction fetch(instruction *IS, int PC)
{
	return IS[PC];
}
