#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 200
#define MAX_SYMBOL_COUNT 50
#define MAX_REG_COUNT 10

// generated code
instruction *code;
int codeIdx;

// symbol table
symbol *table;
int tableIdx;

// lexeme list
lexeme *list;
int listIdx;

int level;
int registerCounter;

void emit(int opname, int reg, int level, int mvalue);
void addToSymbolTable(int k, char n[], int s, int l, int a, int m);
void mark();
int multipleDeclarationCheck(char name[]);
int findSymbol(char name[], int kind);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
int block();
int varDec();
int procedureDec();
int statement();
int condition();
int expression();
int term();
int factor();

// Converted
instruction *parse(lexeme *list, int printTable, int printCode)
{
    // set up program variables
    code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
    codeIdx = 0;
    table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);
    tableIdx = 0;

    // Parsing calls go here
    int registerCounter = -1;
    emit(7, 0, 0, 0);
    addToSymbolTable(3, "main", 0, 0, 0, 0);
    level = -1;

    if (block() == -1)
    {
        printparseerror(0);
        return NULL;
    }

    if (list[listIdx].type != periodsym)
    {
        printparseerror(1);
        return NULL;
    }

    // HLT
    emit(11, 0, 0, 0);
    code[0].m = table[0].addr;
    for (int i = 0; i < codeIdx; i++)
    {
        if (code[i].opcode == 5)
        {
            code[i].m = table[code[i].m].addr;
        }
    }

    // print off table and code
    if (printTable)
        printsymboltable();
    if (printCode)
        printassemblycode();

    // mark the end of the code
    code[codeIdx].opcode = -1;
    return code;
}

// Emits new instruction to generated code array
void emit(int opname, int reg, int level, int mvalue)
{
    code[codeIdx].opcode = opname;
    code[codeIdx].r = reg;
    code[codeIdx].l = level;
    code[codeIdx].m = mvalue;
    codeIdx++;
}

// Emits new symbol to Symbol Table
void addToSymbolTable(int k, char n[], int s, int l, int a, int m)
{
    table[tableIdx].kind = k;
    strcpy(table[tableIdx].name, n);
    table[tableIdx].size = s;
    table[tableIdx].level = l;
    table[tableIdx].addr = a;
    table[tableIdx].mark = m;
    tableIdx++;
}

void mark()
{
    int i;
    for (i = tableIdx - 1; i >= 0; i--)
    {
        if (table[i].mark == 1)
            continue;
        if (table[i].level < level)
            return;
        table[i].mark = 1;
    }
}

int multipleDeclarationCheck(char name[])
{
    int i;
    for (i = 0; i < tableIdx; i++)
        if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
            return i;
    return -1;
}

int findSymbol(char name[], int kind)
{
    int i;
    int max_idx = -1;
    int max_lvl = -1;
    for (i = 0; i < tableIdx; i++)
    {
        if (table[i].mark == 0 && table[i].kind == kind && strcmp(name, table[i].name) == 0)
        {
            if (max_idx == -1 || table[i].level > max_lvl)
            {
                max_idx = i;
                max_lvl = table[i].level;
            }
        }
    }
    return max_idx;
}

void printparseerror(int err_code)
{
    switch (err_code)
    {
    case 1:
        printf("Parser Error: Program must be closed by a period\n");
        break;
    case 2:
        printf("Parser Error: Symbol names must be identifiers\n");
        break;
    case 3:
        printf("Parser Error: Confliciting symbol declarations\n");
        break;
    case 4:
        printf("Parser Error: Array sizes must be given as a single, nonzero number\n");
        break;
    case 5:
        printf("Parser Error: [ must be followed by ]\n");
        break;
    case 6:
        printf("Parser Error: Multiple symbols in var declaration must be separated by commas\n");
        break;
    case 7:
        printf("Parser Error: Symbol declarations should close with a semicolon\n");
        break;
    case 8:
        printf("Parser Error: Procedure declarations should contain a semicolon before the body of the procedure begins\n");
        break;
    case 9:
        printf("Parser Error: Procedures may not be assigned to, read, or used in arithmetic\n");
        break;
    case 10:
        printf("Parser Error: Undeclared identifier\n");
        break;
    case 11:
        printf("Parser Error: Variables cannot be indexed\n");
        break;
    case 12:
        printf("Parserr Error: Arrays must be indexed\n");
        break;
    case 13:
        printf("Parser Error: Assignment operator missing\n");
        break;
    case 14:
        printf("Parser Error: Register Overflow Error\n");
        break;
    case 15:
        printf("Parser Error: call must be followed by a procedure identifier\n");
        break;
    case 16:
        printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
        break;
    case 17:
        printf("Parser Error: begin must be followed by end\n");
        break;
    case 18:
        printf("Parser Error: if must be followed by ?\n");
        break;
    case 19:
        printf("Parser Error: do must be followed by while\n");
        break;
    case 20:
        printf("Parser Error: read must be followed by a var or array identifier\n");
        break;
    case 21:
        printf("Parser Error: Relational operator missing from condition\n");
        break;
    case 22:
        printf("Parser Error: Bad arithmetic\n");
        break;
    case 23:
        printf("Parser Error: ( must be followed by )\n");
        break;
    case 24:
        printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, and variables\n");
        break;
    default:
        printf("Implementation Error: unrecognized error code\n");
        break;
    }

    free(code);
    free(table);
}

void printsymboltable()
{
    int i;
    printf("Symbol Table:\n");
    printf("Kind | Name        | Size | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");
    for (i = 0; i < tableIdx; i++)
        printf("%4d | %11s | %5d | %4d | %5d | %5d\n", table[i].kind, table[i].name, table[i].size, table[i].level, table[i].addr, table[i].mark);

    free(table);
    table = NULL;
}

void printassemblycode()
{
    int i;
    printf("Line\tOP Code\tOP Name\tR\tL\tM\n");
    for (i = 0; i < codeIdx; i++)
    {
        printf("%d\t", i);
        printf("%d\t", code[i].opcode);
        switch (code[i].opcode)
        {
        case 1:
            printf("LIT\t");
            break;
        case 2:
            printf("RET\t");
            break;
        case 3:
            printf("LOD\t");
            break;
        case 4:
            printf("STO\t");
            break;
        case 5:
            printf("CAL\t");
            break;
        case 6:
            printf("INC\t");
            break;
        case 7:
            printf("JMP\t");
            break;
        case 8:
            printf("JPC\t");
            break;
        case 9:
            printf("WRT\t");
            break;
        case 10:
            printf("RED\t");
            break;
        case 11:
            printf("HLT\t");
            break;
        case 12:
            printf("NEG\t");
            break;
        case 13:
            printf("ADD\t");
            break;
        case 14:
            printf("SUB\t");
            break;
        case 15:
            printf("MUL\t");
            break;
        case 16:
            printf("DIV\t");
            break;
        case 17:
            printf("MOD\t");
            break;
        case 18:
            printf("EQL\t");
            break;
        case 19:
            printf("NEQ\t");
            break;
        case 20:
            printf("LSS\t");
            break;
        case 21:
            printf("LEQ\t");
            break;
        case 22:
            printf("GTR\t");
            break;
        case 23:
            printf("GEQ\t");
            break;
        default:
            printf("err\t");
            break;
        }
        printf("%d\t%d\t%d\n", code[i].r, code[i].l, code[i].m);
    }

    if (table != NULL)
        free(table);
}

// Converted
int block()
{
    level++;
    int procedureIdx = tableIdx - 1;
    int x = varDec();
    if (x == -1)
        return -1;
    if (procedureDec() == -1)
        return -1;
    table[procedureIdx].addr = codeIdx;
    // INC
    emit(6, 0, 0, x);
    if (statement() == -1)
        return -1;
    mark();
    level--;

    return 0;
}

// Converted
int varDec()
{
    int memSize = 3;
    char *symName;
    int arraySize;

    if (list[listIdx].type == varsym)
    {
        do
        {
            listIdx++;
            if (list[listIdx].type != identsym)
            {
                printparseerror(2);
                return 1;
            }
            if (multipleDeclarationCheck(list[listIdx].name) != -1)
            {
                printparseerror(3);
                return 1;
            }
            strcpy(symName, list[listIdx].name);
            listIdx++;
            if (list[listIdx].type == lbracketsym)
            {
                listIdx++;
                if (list[listIdx].type != numbersym || list[listIdx].value == 0)
                {
                    printparseerror(4);
                    return 1;
                }
                arraySize = list[listIdx].value;
                listIdx++;
                if (list[listIdx].type == multsym ||
                    list[listIdx].type == divsym ||
                    list[listIdx].type == modsym ||
                    list[listIdx].type == addsym ||
                    list[listIdx].type == subsym)
                {
                    printparseerror(4);
                    return -1;
                }
                else if (list[listIdx].type != rbracketsym)
                {
                    printparseerror(5);
                    return -1;
                }
                listIdx++;
                addToSymbolTable(2, symName, arraySize, level, memSize, 0);
                memSize += arraySize;
            }
            else
            {
                addToSymbolTable(1, symName, 0, level, memSize, 0);
                memSize++;
            }
        } while (list[listIdx].type == commasym);
        if (list[listIdx].type == identsym)
        {
            printparseerror(6);
            return -1;
        }
        else if (list[listIdx].type != semicolonsym)
        {
            printparseerror(7);
            return -1;
        }
        listIdx++;
        return memSize;
    }
    else
    {
        return memSize;
    }
}

// Converted
int procedureDec()
{
    char *symName;

    while (list[listIdx].type == procsym)
    {
        listIdx++;
        if (list[listIdx].type != identsym)
        {
            // error 2
            printparseerror(2);
            return -1;
        }
        else if (multipleDeclarationCheck(list[listIdx].name) != -1)
        {
            // error 3
            printparseerror(3);
            return -1;
        }
        strcpy(symName, list[listIdx].name);
        listIdx++;
        if (list[listIdx].type != semicolonsym)
        {
            // error 8
            printparseerror(8);
            return -1;
        }
        listIdx++;
        addToSymbolTable(3, symName, 0, level, 0, 0);
        if (block() == -1)
        {
            return -1;
        }
        if (list[listIdx].type != semicolonsym)
        {
            // error 7
            printparseerror(7);
            return -1;
        }
        listIdx++;
        emit(2, 0, 0, 0);
    }
    return 0;
}

// Converted
int statement()
{
    char *symName;
    int symIdx;
    int arrayIdxReg;
    int varLocReg;
    int jpcIdx;
    int jmpIdx;
    int loopIdx;

    if (list[listIdx].type == identsym)
    {
        strcpy(symName, list[listIdx].name);
        listIdx++;
        if (list[listIdx].type == lbracketsym)
        {
            listIdx++;
            symIdx = findSymbol(symName, 2);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 1) != -1)
                {
                    printparseerror(11);
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                }
                else
                {
                    printparseerror(10);
                }
                return -1;
            }
            if (expression() == -1)
                return -1;
            arrayIdxReg = registerCounter;
            if (list[listIdx].type != rbracketsym)
            {
                printparseerror(5);
                return -1;
            }
            listIdx++;
            if (list[listIdx].type != assignsym)
            {
                printparseerror(13);
                return -1;
            }
            listIdx++;
            if (expression() == -1)
                return -1;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            emit(13, arrayIdxReg, arrayIdxReg, registerCounter);
            registerCounter--;
            emit(4, registerCounter, level - table[symIdx].level, arrayIdxReg);
            registerCounter -= 2;
        }
        else
        {
            symIdx = findSymbol(symName, 1);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 2) != -1)
                {
                    printparseerror(12);
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                }
                else
                {
                    printparseerror(10);
                }
                return -1;
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            varLocReg = registerCounter;
            if (list[listIdx].type != assignsym)
            {
                printparseerror(13);
                return -1;
            }
            listIdx++;
            if (expression() == -1)
            {
                return -1;
            }
            emit(4, registerCounter, level - table[symIdx].level, varLocReg);
            registerCounter -= 2;
        }
    }

    else if (list[listIdx].type == callsym)
    {
        listIdx++;
        if (list[listIdx].type != identsym)
        {
            printparseerror(15);
            return -1;
        }
        symIdx = findSymbol(list[listIdx].name, 3);
        if (symIdx == -1)
        {
            if (findSymbol(list[listIdx].name, 1) != -1 ||
                findSymbol(list[listIdx].name, 2) != -1)
            {
                printparseerror(15);
            }
            else
            {
                printparseerror(10);
            }
            return -1;
        }
        // CAL
        emit(5, 0, level - table[symIdx].level, symIdx);
        listIdx++;
    }

    else if (list[listIdx].type == beginsym)
    {
        do
        {
            listIdx++;
            if (statement() == -1)
                return -1;
        } while (list[listIdx].type != semicolonsym);
        if (list[listIdx].type != endsym)
        {
            if (list[listIdx].type == identsym ||
                list[listIdx].type == callsym ||
                list[listIdx].type == beginsym ||
                list[listIdx].type == ifsym ||
                list[listIdx].type == dosym ||
                list[listIdx].type == readsym ||
                list[listIdx].type == writesym)
            {
                printparseerror(16);
            }
            else
            {
                printparseerror(17);
            }
            return -1;
        }
        listIdx++;
    }

    else if (list[listIdx].type == ifsym)
    {
        listIdx++;
        if (condition() == -1)
            return -1;
        jpcIdx = codeIdx;
        emit(8, registerCounter, 0, 0);
        registerCounter--;
        if (list[listIdx].type != questionsym)
        {
            printparseerror(18);
            return -1;
        }
        listIdx++;
        if (statement() == -1)
            return -1;
        if (list[listIdx].type == colonsym)
        {
            listIdx++;
            jmpIdx = codeIdx;
            emit(7, 0, 0, 0);
            code[jpcIdx].m = codeIdx;
            if (statement() == -1)
                return -1;
        }

        code[jpcIdx].m = codeIdx;
    }

    else if (list[listIdx].type == dosym)
    {
        listIdx++;
        loopIdx = codeIdx;
        if (statement() == -1)
            return -1;
        if (list[listIdx].type != whilesym)
        {
            printparseerror(19);
            return -1;
        }
        listIdx++;
        if (condition() == -1)
            return -1;
        registerCounter++;
        if (registerCounter >= 10)
        {
            printparseerror(14);
            return -1;
        }
        emit(1, registerCounter, 0, 0);
        emit(18, registerCounter - 1, registerCounter - 1, registerCounter);
        registerCounter--;
        emit(8, registerCounter, 0, loopIdx);
        registerCounter--;
    }

    else if (list[listIdx].type == readsym)
    {
        listIdx++;
        if (list[listIdx].type != identsym)
        {
            printparseerror(20);
            return -1;
        }
        symName = list[listIdx].name;
        listIdx++;
        if (list[listIdx].type == lbracketsym)
        {
            listIdx++;
            symIdx = findSymbol(symName, 2);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 1) != -1)
                {
                    printparseerror(11);
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                }
                else
                {
                    printparseerror(10);
                }
                return -1;
            }
            if (expression() == -1)
                return -1;
            arrayIdxReg = registerCounter;
            if (list[listIdx].type != rbracketsym)
            {
                printparseerror(5);
                return -1;
            }
            listIdx++;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(10, registerCounter, 0, 0);
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            emit(13, arrayIdxReg, arrayIdxReg, registerCounter);
            registerCounter--;
            emit(4, registerCounter, level - table[symIdx].level, arrayIdxReg);
            registerCounter -= 2;
        }
        else
        {
            symIdx = findSymbol(symName, 1);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 2) != -1)
                {
                    printparseerror(12);
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                }
                else
                {
                    printparseerror(10);
                }
                return -1;
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            varLocReg = registerCounter;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(10, registerCounter, 0, 0);
            emit(4, registerCounter, level - table[symIdx].level, varLocReg);
            registerCounter -= 2;
        }
    }

    else if (list[listIdx].type == writesym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emit(9, registerCounter, 0, 0);
        registerCounter--;
    }

    return 0;
}

// Converted
int condition()
{
    int emitVal;
    if (expression() == -1)
        return -1;

    if (list[listIdx].type == eqlsym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emitVal = 18;
    }
    else if (list[listIdx].type == neqsym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emitVal = 19;
    }
    else if (list[listIdx].type == lsssym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emitVal = 20;
    }
    else if (list[listIdx].type == leqsym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emitVal = 21;
    }
    else if (list[listIdx].type == gtrsym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emitVal = 22;
    }
    else if (list[listIdx].type == geqsym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        emitVal = 23;
    }
    else
    {
        printparseerror(21);
        return -1;
    }

    emit(emitVal, registerCounter - 1, registerCounter - 1, registerCounter);
    registerCounter--;

    return 0;
}

// Converted
int expression()
{
    int emitVal;

    if (list[listIdx].type == subsym)
    {
        listIdx++;
        if (term() == -1)
            return -1;
        emit(12, registerCounter, 0, registerCounter);
    }
    else
    {
        if (term() == -1)
            return -1;
    }
    while (list[listIdx].type == addsym || list[listIdx].type == subsym)
    {
        listIdx++;
        if (term() == -1)
            return -1;

        emitVal = (list[listIdx].type == addsym) ? 13 : 14;

        emit(emitVal, registerCounter - 1, registerCounter - 1, registerCounter);
        registerCounter--;
    }
    if (list[listIdx].type == lparenthesissym ||
        list[listIdx].type == identsym ||
        list[listIdx].type == numbersym)
    {
        printparseerror(22);
        return -1;
    }
    return 0;
}

// Converted
int term()
{
    int emitVal;

    if (factor() == -1)
        return -1;

    while (list[listIdx].type == multsym ||
           list[listIdx].type == divsym ||
           list[listIdx].type == modsym)
    {
        listIdx++;
        if (factor() == -1)
            return -1;

        if (list[listIdx].type == multsym)
        {
            emitVal = 15;
        }
        else if (list[listIdx].type == divsym)
        {
            emitVal = 16;
        }
        else
        {
            emitVal = 17;
        }
        emit(emitVal, registerCounter - 1, registerCounter - 1, registerCounter);
        registerCounter--;
    }
    return 0;
}

// Converted
int factor()
{
    char *symName;
    int symIdx;
    int arrayIdxRegister;
    int varLocReg;

    if (list[listIdx].type == identsym)
    {
        strcpy(symName, list[listIdx].name);
        listIdx++;
        if (list[listIdx].type == lbracketsym)
        {
            listIdx++;
            symIdx = findSymbol(symName, 2);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 1) != -1)
                {
                    printparseerror(11);
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                }
                else
                {
                    printparseerror(10);
                }
                return -1;
            }
            if (expression() == -1)
                return -1;
            arrayIdxRegister = registerCounter;
            if (list[listIdx].type != rbracketsym)
            {
                printparseerror(5);
                return -1;
            }
            listIdx++;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            // LIT
            emit(1, registerCounter, 0, table[symIdx].addr);
            // ADD
            emit(13, arrayIdxRegister, arrayIdxRegister, registerCounter);
            registerCounter--;
            // LOD
            emit(3, registerCounter, level - table[symIdx].level, arrayIdxRegister);
        }
        else
        {
            symIdx = findSymbol(symName, 1);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 2) != -1)
                {
                    printparseerror(12);
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                }
                else
                {
                    printparseerror(10);
                }
                return -1;
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return -1;
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            varLocReg = registerCounter;
            emit(3, registerCounter, level - table[symIdx].level, varLocReg);
        }
    }
    else if (list[listIdx].type == numbersym)
    {
        registerCounter++;
        if (registerCounter >= 10)
        {
            printparseerror(14);
            return -1;
        }
        emit(1, registerCounter, 0, list[listIdx].value);
        listIdx++;
    }
    else if (list[listIdx].type == lparenthesissym)
    {
        listIdx++;
        if (expression() == -1)
            return -1;
        if (list[listIdx].type != rparenthesissym)
        {
            printparseerror(23);
            return -1;
        }
        listIdx++;
    }
    else
    {
        printparseerror(24);
        return -1;
    }

    return 0;
}
