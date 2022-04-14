#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 200
#define MAX_SYMBOL_COUNT 50
#define MAX_REG_COUNT 10

// generated code
instruction *code;
int codeIndex;

// symbol table
symbol *table;
int tableIndex;

// lexeme list
lexeme *list;
int listIdx;

int errCode;
int level;
int registerCounter;
int M;

void emit(int opname, int reg, int level, int mvalue);
void addToSymbolTable(int k, char n[], int s, int l, int a, int m);
void mark();
int multipledeclarationcheck(char name[]);
int findsymbol(char name[], int kind);
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
    codeIndex = 0;
    table = malloc(sizeof(symbol) * MAX_SYMBOL_COUNT);
    tableIndex = 0;

    errCode = 0;

    // Parsing calls go here
    int registerCounter = -1;
    emit(7, 0, 0, 0);
    addToSymbolTable(3, "main", 0, 0, 0, 0);
    level = -1;

    if (errCode = block())
    {
        printparseerror(errCode);
        return NULL;
    }

    emit(11, 0, 0, 0);
    code[0].m = table[0].addr;
    for (int i = 0; i < codeIndex; i++)
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
    code[codeIndex].opcode = -1;
    return code;
}

int varDec()
{
    int memSize = 3;
    char *symName;

    if (list[listIdx] == varsym)
    {
        listIdx++;
        if (list[listIdx] != identsym)
        {
            return 2; // TODO
        }
        if (multipledeclarationcheck(list[listIdx].name]) != -1)
        {
            return 3; // TODO
        }
        strcpy(symName, list[listIdx].name);
        listIdx++;
        if (list[listIdx] == lbracketsym)
        {
            listIdx++;
            if (list[listIdx] != numbersym || list[listIdx].value == 0)
            {
                return 4;
            }
            arraySize = list[listIdx].value;
        }
    }
}

// Emits new instruction to generated code array
void emit(int opname, int reg, int level, int mvalue)
{
    code[codeIndex].opcode = opname;
    code[codeIndex].r = reg;
    code[codeIndex].l = level;
    code[codeIndex].m = mvalue;
    codeIndex++;
}

// Emits new symbol to Symbol Table
void addToSymbolTable(int k, char n[], int s, int l, int a, int m)
{
    table[tableIndex].kind = k;
    strcpy(table[tableIndex].name, n);
    table[tableIndex].size = s;
    table[tableIndex].level = l;
    table[tableIndex].addr = a;
    table[tableIndex].mark = m;
    tableIndex++;
}

void mark()
{
    int i;
    for (i = tableIndex - 1; i >= 0; i--)
    {
        if (table[i].mark == 1)
            continue;
        if (table[i].level < level)
            return;
        table[i].mark = 1;
    }
}

int multipledeclarationcheck(char name[])
{
    int i;
    for (i = 0; i < tableIndex; i++)
        if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
            return i;
    return -1;
}

int findsymbol(char name[], int kind)
{
    int i;
    int max_idx = -1;
    int max_lvl = -1;
    for (i = 0; i < tableIndex; i++)
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
    for (i = 0; i < tableIndex; i++)
        printf("%4d | %11s | %5d | %4d | %5d | %5d\n", table[i].kind, table[i].name, table[i].size, table[i].level, table[i].addr, table[i].mark);

    free(table);
    table = NULL;
}

void printassemblycode()
{
    int i;
    printf("Line\tOP Code\tOP Name\tR\tL\tM\n");
    for (i = 0; i < codeIndex; i++)
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
    procedureDec()
        table[procedureIdx]
            .addr = codeIndex;
    emit(6, 0, 0, M = x);
    statement();
    mark();
    level--;
}

// Converted
int varDec()
{
    int memSize = 3;
    char *symName;
    int arraySize;

    if (list[listIdx] == varsym)
    {
        do
        {
            listIdx++;
            if (list[listIdx] != identsym)
            {
                return 2;
            }
            if (multipledeclarationcheck(list[listIdx].name) != -1)
            {
                return 3;
            }
            strcpy(symName, list[listIdx].name);
            listIdx++;
            if (list[listIdx] == lbracketsym)
            {
                listIdx++;
                if (list[listIdx] != numbersym || list[listIdx].value == 0)
                {
                    printparseerror(3);
                    return 0;
                }
                arraySize = list[listIdx].value;
                listIdx++;
                if (list[listIdx] == multsym ||
                    list[listIdx] == divsym ||
                    list[listIdx] == modsym ||
                    list[listIdx] == addsym ||
                    list[listIdx] == subsym)
                {
                    printparseerror(4);
                    return 0;
                }
                else if (list[listIdx] != rbracketsym)
                {
                    printparseerror(5);
                    return 0;
                }
                listIdx++;
                addToSymbolTable(2, symName, arraySize, level, memSize, 0);
                memSize != arraySize;
            }
            else
            {
                addToSymbolTable(1, symName, 0, level, memSize, 0);
                memSize++;
            }
        } while (list[listIdx] == commasym);
        if (list[listIdx] == identsym)
        {
            printparseerror(6);
            return 0;
        }
        else if (list[listIdx] != semicolonsym)
        {
            printparseerror(7);
            return 0;
        }
        listIdx++;
        return memSize;
    }
    else
    {
        return memSize;
    }
}
int procedureDec()
{
    /*
       symbolname
       while list[listidx] is proceduresym
           listidx++
           if list[listidx] is not identsym
               error 2
           else if multipledeclarationcheck(list[listidx].name) != -1
               error 3
           symbolname = list[listidx].name
           listidx++
           if list[listidx] is not semicolonsym
               error 8
           listidx++
           addtosymboltable(3, symbolname, 0, level, 0, 0)
           block()
           if list[listidx] is not semicolonsym
               error 7
           listidx++
           emit RET (2, 0, 0, 0)
    */
}
int statement()
{
    /*
           symbolname
       symidx
       arrayidxreg
       varlocreg
       jpcidx
       jmpidx
       loopidx
       assignment
           symbolname = list[listidx].name
           listidx++
           if list[listidx] is lbracketsym
               listidx++
               symidx = findsymbol(symbolname, 2)
               if symidx == -1
                   if findsymbol(symbolname, 1) != -1
                       error 11
                   else if findsymbol(symbolname, 3) != -1
                       error 9
                   else
                       error 10
               expression()
               arrayidxreg = registercounter
               if list[listidx] is not rbracketsym
                   error 5
               listidx++
               if list[listidx] is not assignsym
                   error 13
               listidx++
               expression()
               registercounter++
               if registercounter >= 10
                   error 14
               emit LIT (1, R = registercounter, 0, M = table[symidx].addr)
               emit ADD (13, R = arrayidxreg, L = arrayidxreg, registercounter)
               registercounter--
               emit STO (4, R = registercounter, L = level - table[symidx].level, M = arrayidxreg)
               registercounter -= 2
           else
               symidx = findsymbol(symbolname, 1)
               if symidx == -1
                   if findsymbol(symbolname, 2) != -1
                       error 12
                   else if findsymbol(symbolname, 3) != -1
                       error 9
                   else
                       error 10
               registercounter++
               if registercounter >= 10
                   error 14
               emit LIT(1, R = registercounter, 0, M = table[symidx].addr)
               varlocreg = registercounter
               if list[listidx] is not assignsym
                   error 13
               listidx++
               expression()
               emit STO (4, R = registercounter, L = level - table[symidx].level, M = varlocreg)
               registercounter -= 2
       call
           listidx++
           if list[listidx] is not identsym
               error 15
           symidx = findsymbol(list[listidx].name, 3)
           if symidx == -1
               if findsymbol(list[listidx].name, 1) != -1 || findsymbol(list[listidx].name, 2) != -1
                   error 15
               else
                   error 10
           emit CAL (5, 0, L = level - table[symidx].level, M = symidx)
           listidx++
       begin-end
           do
               listidx++
               statement()
           while list[listidx] is semicolonsym
           if list[listidx] is not endsym
               if list[listidx] is identsym, callsym, beginsym, ifsym, dosym, readsym, or writesym
                   error 16
               else
                   error 17
           listidx++
       if
           listidx++
           condition()
           jpcidx = codeidx
           emit JPC (8, R = registercounter, 0, M = 0)
           registercounter--
           if list[listidx] is not questionsym
               error 18
           listidx++
           statement()
           if list[listidx] is colonsym
               listidx++
               jmpidx = codeidx
               emit JMP (7, 0, 0, M = 0)
               code[jpcidx].m = codeidx
               statement()
               code[jmpidx].m = codeidx
           else
               code[jpcidx].m = codeidx
       do-while
           listidx++
           loopidx = codeidx
           statement()
           if list[listidx] is not whilesym
               error 19
           listidx++
           condition()
           registercounter++
           if registercounter >= 10
               error 14
           emit LIT(1, R = registercounter, 0, M = 0)
           emit EQL (18, R = registercounter - 1, L = registercounter - 1, M = registercounter)
           registercounter--
           emit JPC (8, R = registercounter, 0, M = loopidx)
           registercounter--
       read
           listidx++
           if list[listidx] is not identsym
               error 20
           symbolname = list[listidx].name
           listidx++
           if list[listidx] is lbracketsym
               listidx++
               symidx = findsymbol(symbolname, 2)
               if symidx == -1
                   if findsymbol(symbolname, 1) != -1
                       error 11
                   else if findsymbol(symbolname, 3) != -1
                       error 9
                   else
                       error 10
               expression()
               arrayidxreg = registercounter
               if list[listidx] is not rbracketsym
                   error 5
               listidx++
               registercounter++
               if registercounter >= 10
                   error 14
               emit RED (10, R = registercounter, 0, 0)
               registercounter++
               if registercounter >= 10
                   error 14
               emit LIT (1, R = registercounter, 0, table[symidx].addr)
               emit ADD (13, R = arrayidxreg, L = arrayidxreg, M = registercounter)
               registercounter--
               emit STO (4, R = registercounter, L = level - table[symidx].level, M = arrayidxreg)
               registercounter -= 2
           else
               symidx = findsymbol(symbolname, 1)
               if symidx == -1
                   if findsymbol(symbolname, 2) != -1
                       error 12
                   else if findsymbol(symbolname, 3) != -1
                       error 9
                   else
                       error 10
               registercounter++
               if registercounter >= 10
                   error 14
               emit LIT (1, R = registercounter, 0, table[symidx].addr)
               varlocreg = registercounter
               registercounter++
               if registercounter >= 10
                   error 14
               emit RED (10, R = registercounter, 0, 0)
               emit STO (4, R = registercounter, L = level - table[symidx].level, M = varlocreg)
               registercounter -= 2
       write
           listidx++
           expression()
           emit WRT (9, R = registercounter, 0, 0)
           registercounter--
    */
}
// Converted
int condition()
{
    expression() if (list[listIdx] == eqlsym)
    {
        listIdx++;
        expression();
        emit EQL(18, R = registercounter - 1, L = registercounter - 1, M = registercounter);
        registercounter--;
    }
    else if (list[listIdx] == neqsym)
    {
        listIdx++;
        expression();
        emit NEQ(19, R = registercounter - 1, L = registercounter - 1, M = registercounter);
        registercounter--;
    }
    else if (list[listIdx] == lsssym)
    {
        listIdx++;
        expression();
        emit LSS(20, R = registercounter - 1, L = registercounter - 1, M = registercounter);
        registercounter--;
    }
    else if list[listidx] is leqsym
        listidx++;
    expression();
    emit LEQ(21, R = registercounter - 1, L = registercounter - 1, M = registercounter);
    registercounter--;
    else if (list[listIdx] == gtrsym)
    {
        listIdx++;
        expression();
        emit GTR(22, R = registercounter - 1, L = registercounter - 1, M = registercounter);
        registercounter--;
    }
    else if (list[listidx] == geqsym)
    {
        listIdx++;
        expression();
        emit GEQ(23, R = registercounter - 1, L = registercounter - 1, M = registercounter);
        registercounter--;
    }
    else
    {
        // error 21
        return 21;
    }
    return 0;
}
// Converted
int expression()
{
    if (list[listIdx] == subsym)
    {
        listIdx++;
        term();
        emit NEG(12, R = registercounter, 0, M = registercounter);

        while (list[listIdx] == addsym || subsym)
        {
            if (list[listIdx] == addsym)
            {
                listIdx++;
                term();
                emit ADD(13, R = registercounter - 1, L = registercounter - 1, M = registercounter);
                registercounter--;
            }
            else
            {
                listIdx++;
                term();
                emit SUB(14, R = registercounter - 1, L = registercounter - 1, M = registercounter);
                registercounter--;
            }
        }
    }
    else
    {
        term();
        while (list[listIdx] == addsym || subsym)
        {
            if (list[listIdx] == addsym)
            {
                listIdx++;
                term();
                emit ADD(13, R = registercounter - 1, L = registercounter - 1, M = registercounter);
                registercounter--;
            }
            else
            {
                listIdx++;
                term();
                emit SUB(14, R = registercounter - 1, L = registercounter - 1, M = registercounter);
                registercounter--;
            }
        }
    }
    if list
        [listidx] is lparenthesissym, identsym, or numbersym
        {
            // error 22
            return 22;
        }
    return 0;
}
// Converted
int term()
{
    factor();
    while (list[listIdx] == multsym || list[listIdx] == divsymlist || [listIdx] == modsym)
    {
        if (list[listIdx] == multsym)
        {
            listIdx++;
            factor();
            emit MUL(15, R = registercounter - 1, L = registercounter - 1, M = registercounter);
            registercounter--;
        }
        else if (list[listIdx] == divsym)
        {
            listIdx++;
            factor();
            emit DIV(16, R = registercounter - 1, L = registercounter - 1, M = registercounter);
            registercounter--;
        }
        else
        {
            listIdx++;
            factor();
            emit MOD(17, R = registercounter - 1, L = registercounter - 1, M = registercounter);
            registercounter--;
        }
    }
}
// Converted
int factor()
{
    char *symName;
    int symIdx;
    int arrayIdxRegister;
    int varLocReg;

    if (list[listIdx] == identsym)
    {
        strcpy(symName, list[listIdx].name);
        listIdx++;
        if (list[listIdx] == lbracketsym)
        {
            listIdx++;
            symIdx = findSymbol(symName, 2);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 1) != -1)
                {
                    printparseerror(11);
                    return 0;
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                    return 0;
                }
                else
                {
                    printparseerror(10);
                    return 0;
                }
            }
            expression();
            arrayIdxRegister = registerCounter;
            if (list[listIdx] != rbracketsym)
            {
                printparseerror(5);
                return 0;
            }
            listIdx++;
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return 0;
            }
            emit(1, R = registerCounter, 0, M = table[symIdx].addr);
            emit(13, R = arrayIdxRegister, L = arrayIdxRegister, M = registerCounter);
            registerCounter--;
            emit(3, R = registerCounter, L = level - table[symIdx].level, M = arrayIdxRegister);
        }
        else
        {
            symIdx = findSymbol(symName, 1);
            if (symIdx == -1)
            {
                if (findSymbol(symName, 2) != -1)
                {
                    printparseerror(12);
                    return 0;
                }
                else if (findSymbol(symName, 3) != -1)
                {
                    printparseerror(9);
                    return 0;
                }
                else
                {
                    printparseerror(10);
                    return 0;
                }
            }
            registerCounter++;
            if (registerCounter >= 10)
            {
                printparseerror(14);
                return 0;
            }
            emit(1, R = registerCounter, 0, M = table[symIdx].addr);
            varLocReg = registerCounter;
            emit(3, R = registerCounter, L = level - table[symIdx].level, M = varLocReg);
        }
    }
    else if (list[listIdx] == numbersym)
    {
        registerCounter++;
        if (registerCounter >= 10)
        {
            printparseerror(14);
            return 0;
        }
        emit(1, R = registerCounter, 0, M = list[listIdx].value);
        listIdx++;
    }
    else if (list[listIdx] == lparenthesissym)
    {
        listIdx++;
        expression();
        if (list[listIdx] != rparenthesissym)
        {
            printparseerror(23);
            return 0;
        }
        listIdx++;
    }
    else
    {
        printparseerror(24);
        return 0;
    }
}
