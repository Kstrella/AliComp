// Systems Software - Prof. Ali Almalki
// Spring 2022
// Authors: Justice Smith, Kevin Estrella

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define REG_FILE_SIZE 10
#define MAX_STACK_LENGTH 100

// opcodes start at 1, so shifted with stand-in 'err'
char *opname[] = {
	"ERR",
	"LIT",
	"RET",
	"LOD",
	"STO",
	"CAL",
	"INC",
	"JMP",
	"JPC",
	"WRT",
	"RED",
	"HAL",
	"NEG",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"EQL",
	"NEQ",
	"LSS",
	"LEQ",
	"GTR",
	"GEQ"};

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

// Returns: instruction as defined in 'compiler.h'
// example call:
// 		fetch(IS, PC);
instruction fetch(instruction *IS, int PC)
{
	return IS[PC];
}

void execute_program(instruction *code, int printFlag)
{
	// Memory Stack
	int *stack = (int *)calloc(MAX_STACK_LENGTH, sizeof(int));
	// Register File
	int *RF = (int *)calloc(REG_FILE_SIZE, sizeof(int));
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

	int halt = 0;
	int line = 0;

	while (halt == 0)
	{
		IR = fetch(code, PC);

		switch (IR.opcode)
		{
		// LIT
		case 1:
			RF[IR.r] = IR.m;
			break;
		// RET
		case 2:
			// Return from current procedure (X) to the last procedure (Y).
			// SP = the index of the end of Y’s AR (BP + 1)
			SP = BP + 1;
			// PC = return address value from X’s AR
			PC = stack[BP - 2];

			// !!NOTE must change BP last or dep issue with setting above
			// BP = dynamic link value from X’s AR
			BP = stack[BP - 1];
			break;
		// LOD
		case 3:
			if (base(IR.l, BP, stack) - IR.m < 0 || base(IR.l, BP, stack) - IR.m >= MAX_STACK_LENGTH)
			{
				printf("Virtual Machine Error: Out of Bounds Access Error\n");
				halt = 1;
				break;
			}
			// Load value to register IR.r from the stack location at offset RF[IR.m] from L levels up
			RF[IR.r] = stack[base(IR.l, BP, stack) - RF[IR.m]];
			break;
		// STO
		case 4:
			if (base(IR.l, BP, stack) - IR.m < 0 || base(IR.l, BP, stack) - IR.m >= MAX_STACK_LENGTH)
			{
				printf("Virtual Machine Error: Out of Bounds Access Error\n");
				halt = 1;
				break;
			}
			stack[base(IR.l, BP, stack) - RF[IR.m]] = RF[IR.r];
			break;
		// CAL
		case 5:
			// static link
			stack[SP - 1] = base(IR.l, BP, stack);
			// dynamic link
			stack[SP - 2] = BP;
			// return address
			stack[SP - 3] = PC + 1;

			BP = SP - 1;
			PC = IR.m;
			break;
		// INC
		case 6:
			// Account for linking indices ??
			SP -= IR.m;
			if (SP < 0)
			{
				printf("Virtual Machine Error: Stack Overflow Error\n");
				halt = 1;
			}
			break;
		// JMP
		case 7:
			PC = IR.m;
			break;
		// JPC
		case 8:
			if (RF[IR.r] == 0)
			{
				PC = IR.m;
			}
			else
			{
				PC++;
			}
			break;
		// WRT
		case 9:
			printf("%d", RF[IR.r]);
			break;
		// RED
		case 10:
			scanf("%d", &RF[IR.r]);
			break;
		// HAL
		case 11:
			halt = 1;
			break;
		// NEG
		case 12:
			RF[IR.r] = RF[IR.r] * -1;
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
			RF[IR.r] = (RF[IR.l] <= RF[IR.m]) ? 1 : 0;
			break;
		// GTR
		case 22:
			RF[IR.r] = (RF[IR.l] > RF[IR.m]) ? 1 : 0;
			break;
		// GEQ
		case 23:
			RF[IR.r] = (RF[IR.l] >= RF[IR.m]) ? 1 : 0;
			break;
		default:
			break;
		}

		if (!(IR.opcode == 2 || // RET
			  IR.opcode == 5 || // CAL
			  IR.opcode == 7 || // JMP
			  IR.opcode == 8))	// JPC
		{
			PC++;
		}

		// We only want to print when not halt or when instr is HALT(valid state)
		// All other halted states are errors and will break if printed.
		if (printFlag && (!halt || opname[IR.opcode] == "HAL"))
		{
			print_execution(line, opname[IR.opcode], IR, PC, BP, SP, stack, RF);
		}

		line++;
	}

	free(stack);
	free(RF);
}
