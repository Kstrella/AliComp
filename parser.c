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

// lexeme lexList
lexeme *lexList;
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
void block();
int varDec();
void procedureDec();
void statement();
void condition();
void expression();
void term();
void factor();

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
    lexList = list;

    block();

    if (lexList[listIdx].type != periodsym)
    {
        printparseerror(1);
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
void block()
{
    level++;
    int procedureIdx = tableIdx - 1;
    int x = varDec();
    procedureDec();
    table[procedureIdx].addr = codeIdx;
    // INC
    emit(6, 0, 0, x);
    statement();
    mark();
    level--;
}

// Converted
int varDec()
{
    int memSize = 3;
    char *symName = malloc(sizeof(char) * 12);
    int arraySize;

    if (lexList[listIdx].type == varsym)
    {
        do
        {
            listIdx++;
            if (lexList[listIdx].type != identsym)
            {
                printparseerror(2);
                
            }
            if (multipleDeclarationCheck(lexList[listIdx].name) != -1)
            {
                printparseerror(3);
                
            }
            strcpy(symName, lexList[listIdx].name);
            listIdx++;
            if (lexList[listIdx].type == lbracketsym)
            {
                listIdx++;
                if (lexList[listIdx].type != numbersym || lexList[listIdx].value == 0)
                {
                    printparseerror(4);
                    
                }
                arraySize = lexList[listIdx].value;
                listIdx++;
                if (lexList[listIdx].type == multsym ||
                    lexList[listIdx].type == divsym ||
                    lexList[listIdx].type == modsym ||
                    lexList[listIdx].type == addsym ||
                    lexList[listIdx].type == subsym)
                {
                    printparseerror(4);
                    
                }
                else if (lexList[listIdx].type != rbracketsym)
                {
                    printparseerror(5);
                    
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
        } while (lexList[listIdx].type == commasym);
        if (lexList[listIdx].type == identsym)
        {
            printparseerror(6);
            
        }
        else if (lexList[listIdx].type != semicolonsym)
        {
            printparseerror(7);
            
        }
        listIdx++;
    }
    
    return memSize;
}

// Converted
void procedureDec()
{
    char *symName = malloc(sizeof(char) * 12);

    while (lexList[listIdx].type == procsym)
    {
        listIdx++;
        if (lexList[listIdx].type != identsym)
        {
            // error 2
            printparseerror(2);
            
        }
        else if (multipleDeclarationCheck(lexList[listIdx].name) != -1)
        {
            // error 3
            printparseerror(3);
            
        }
        strcpy(symName, lexList[listIdx].name);
        listIdx++;
        if (lexList[listIdx].type != semicolonsym)
        {
            // error 8
            printparseerror(8);
            
        }

        listIdx++;
        addToSymbolTable(3, symName, 0, level, 0, 0);
        block();

        if (lexList[listIdx].type != semicolonsym)
        {
            // error 7
            printparseerror(7);
            
        }
        listIdx++;
        emit(2, 0, 0, 0);
    }
}

// Converted
void statement()
{
    char *symName = malloc(sizeof(char) * 12);
    int symIdx;
    int arrayIdxReg;
    int varLocReg;
    int jpcIdx;
    int jmpIdx;
    int loopIdx;

    if (lexList[listIdx].type == identsym)
    {
        strcpy(symName, lexList[listIdx].name);
        listIdx++;
        if (lexList[listIdx].type == lbracketsym)
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
            }
            expression();
            arrayIdxReg = registerCounter;
            if (lexList[listIdx].type != rbracketsym)
            {
                printparseerror(5);
            }
            listIdx++;
            if (lexList[listIdx].type != assignsym)
            {
                printparseerror(13);
            }
            listIdx++;
            expression();
            if (registerCounter >= 10)
            {
                printparseerror(14);
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
                
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            varLocReg = registerCounter;
            if (lexList[listIdx].type != assignsym)
            {
                printparseerror(13);
                
            }
            listIdx++;
            expression();
            emit(4, registerCounter, level - table[symIdx].level, varLocReg);
            registerCounter -= 2;
        }
    }

    else if (lexList[listIdx].type == callsym)
    {
        listIdx++;
        if (lexList[listIdx].type != identsym)
        {
            printparseerror(15);
        }
        symIdx = findSymbol(lexList[listIdx].name, 3);
        if (symIdx == -1)
        {
            if (findSymbol(lexList[listIdx].name, 1) != -1 ||
                findSymbol(lexList[listIdx].name, 2) != -1)
            {
                printparseerror(15);
            }
            else
            {
                printparseerror(10);
            }
        }
        // CAL
        emit(5, 0, level - table[symIdx].level, symIdx);
        listIdx++;
    }

    else if (lexList[listIdx].type == beginsym)
    {
        do
        {
            listIdx++;
            statement();
        } while (lexList[listIdx].type != semicolonsym);
        if (lexList[listIdx].type != endsym)
        {
            if (lexList[listIdx].type == identsym ||
                lexList[listIdx].type == callsym ||
                lexList[listIdx].type == beginsym ||
                lexList[listIdx].type == ifsym ||
                lexList[listIdx].type == dosym ||
                lexList[listIdx].type == readsym ||
                lexList[listIdx].type == writesym)
            {
                printparseerror(16);
            }
            else
            {
                printparseerror(17);
            }
        }
        listIdx++;
    }

    else if (lexList[listIdx].type == ifsym)
    {
        listIdx++;
        condition();
        jpcIdx = codeIdx;
        emit(8, registerCounter, 0, 0);
        registerCounter--;
        if (lexList[listIdx].type != questionsym)
        {
            printparseerror(18);
            
        }
        listIdx++;
        statement();
        if (lexList[listIdx].type == colonsym)
        {
            listIdx++;
            jmpIdx = codeIdx;
            emit(7, 0, 0, 0);
            code[jpcIdx].m = codeIdx;
            statement();
        }

        code[jpcIdx].m = codeIdx;
    }

    else if (lexList[listIdx].type == dosym)
    {
        listIdx++;
        loopIdx = codeIdx;
        statement();
        if (lexList[listIdx].type != whilesym)
        {
            printparseerror(19);
        }
        listIdx++;
        condition();
        registerCounter++;
        if (registerCounter >= 10)
        {
            printparseerror(14);
            
        }
        emit(1, registerCounter, 0, 0);
        emit(18, registerCounter - 1, registerCounter - 1, registerCounter);
        registerCounter--;
        emit(8, registerCounter, 0, loopIdx);
        registerCounter--;
    }

    else if (lexList[listIdx].type == readsym)
    {
        listIdx++;
        if (lexList[listIdx].type != identsym)
        {
            printparseerror(20);
            
        }
        strcpy(symName, lexList[listIdx].name);
        listIdx++;
        if (lexList[listIdx].type == lbracketsym)
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
                
            }
            expression();
            arrayIdxReg = registerCounter;
            if (lexList[listIdx].type != rbracketsym)
            {
                printparseerror(5);
                
            }
            listIdx++;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
            }
            emit(10, registerCounter, 0, 0);
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
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
                
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            varLocReg = registerCounter;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
            }
            emit(10, registerCounter, 0, 0);
            emit(4, registerCounter, level - table[symIdx].level, varLocReg);
            registerCounter -= 2;
        }
    }

    else if (lexList[listIdx].type == writesym)
    {
        listIdx++;
        expression();
        emit(9, registerCounter, 0, 0);
        registerCounter--;
    }
}

// Converted
void condition()
{
    int emitVal;
    expression();

    if (lexList[listIdx].type == eqlsym)
    {
        listIdx++;
        expression();
        emitVal = 18;
    }
    else if (lexList[listIdx].type == neqsym)
    {
        listIdx++;
        expression();
        emitVal = 19;
    }
    else if (lexList[listIdx].type == lsssym)
    {
        listIdx++;
        expression();
        emitVal = 20;
    }
    else if (lexList[listIdx].type == leqsym)
    {
        listIdx++;
        expression();
        emitVal = 21;
    }
    else if (lexList[listIdx].type == gtrsym)
    {
        listIdx++;
        expression();
        emitVal = 22;
    }
    else if (lexList[listIdx].type == geqsym)
    {
        listIdx++;
        expression();
        emitVal = 23;
    }
    else
    {
        printparseerror(21);
        
    }

    emit(emitVal, registerCounter - 1, registerCounter - 1, registerCounter);
    registerCounter--;

}

// Converted
void expression()
{
    int emitVal;

    if (lexList[listIdx].type == subsym)
    {
        listIdx++;
        term();
        emit(12, registerCounter, 0, registerCounter);
    }
    else
    {
        term();
    }
    while (lexList[listIdx].type == addsym || lexList[listIdx].type == subsym)
    {
        listIdx++;
        term();

        emitVal = (lexList[listIdx].type == addsym) ? 13 : 14;

        emit(emitVal, registerCounter - 1, registerCounter - 1, registerCounter);
        registerCounter--;
    }
    if (lexList[listIdx].type == lparenthesissym ||
        lexList[listIdx].type == identsym ||
        lexList[listIdx].type == numbersym)
    {
        printparseerror(22);
        
    }
}

// Converted
void term()
{
    int emitVal;

    factor();

    while (lexList[listIdx].type == multsym ||
           lexList[listIdx].type == divsym ||
           lexList[listIdx].type == modsym)
    {
        listIdx++;
        factor();

        if (lexList[listIdx].type == multsym)
        {
            emitVal = 15;
        }
        else if (lexList[listIdx].type == divsym)
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
}

// Converted
void factor()
{
    char *symName = malloc(sizeof(char) * 12);
    int symIdx;
    int arrayIdxRegister;
    int varLocReg;

    if (lexList[listIdx].type == identsym)
    {
        strcpy(symName, lexList[listIdx].name);
        listIdx++;
        if (lexList[listIdx].type == lbracketsym)
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
                
            }
            expression();
            arrayIdxRegister = registerCounter;
            if (lexList[listIdx].type != rbracketsym)
            {
                printparseerror(5);
                
            }
            listIdx++;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
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
                
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                
            }
            emit(1, registerCounter, 0, table[symIdx].addr);
            varLocReg = registerCounter;
            emit(3, registerCounter, level - table[symIdx].level, varLocReg);
        }
    }
    else if (lexList[listIdx].type == numbersym)
    {
        registerCounter++;
        if (registerCounter >= 10)
        {
            printparseerror(14);
            
        }
        emit(1, registerCounter, 0, lexList[listIdx].value);
        listIdx++;
    }
    else if (lexList[listIdx].type == lparenthesissym)
    {
        listIdx++;
        expression();
        if (lexList[listIdx].type != rparenthesissym)
        {
            printparseerror(23);
            
        }
        listIdx++;
    }
    else
    {
        printparseerror(24);
        
    }
}
