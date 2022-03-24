#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;

int alphatoken();
int numbertoken();
int symboltoken();
int comment();
int reservedcheck(char *buffer);

void printlexerror(int type);
void printtokens();

lexeme *lexanalyzer(char *input, int printFlag)
{
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	lex_index = 0;

	if (printFlag)
		printtokens();
	list[lex_index++].type = -1;

	int file_len = strlen(input);
	char ident_buf[MAX_IDENT_LEN + 1];
	int ident_buf_idx = 0;

	// iterate over input
	while (file_index <= file_len)
	{
		// check for space/cntrl
		if (isspace(input[file_index]) || iscntrl(input[file_index])
		{
			lex_index++;
		}
		else if (isalpha(input[file_index]))
		{
			ident_buf[ident_buf_idx++] = input[file_index++];

			while (ident_buf_idx < MAX_NUM_LEN &&
				   (isdigit(input[file_index]) ||
					isalpha(input[file_index])))
			{
				ident_buf[ident_buf_idx++] = input[file_index++];
			}

			if (ident_buf_idx > MAX_NUM_LEN)
			{
				printlexerror(1);
				return NULL;
			}
			else
			{
				if (!isReserved(ident_buf))
				{
					list[lex_index].type = identsym;
					list[lex_index].value = identsym;
					strcpy(list[lex_index++].name, ident_buf);
				}
			}
		}
		else if (isdigit(input[file_index]))
		{
			ident_buf[ident_buf_idx++] = input[file_index++];

			while (ident_buf_idx < MAX_NUM_LEN && isdigit(input[file_index]))
			{
				ident_buf[ident_buf_idx++] = input[file_index++];
			}
			if (ident_buf_idx >= MAX_NUM_LEN)
			{
				printlexerror(2);
				return NULL;
			}
			else if (isalpha(input[file_index])
			{
				printlexerror(1);
				return NULL;
			}
			else
			{
				list[lex_index].type = numbersym;
				strcpy(list[lex_index].name, "numbersym");
				list[lex_index++].value = atoi(ident_buf);

				reset_buf(ident_buf, &ident_buf_idx);
			}
		}	
		else // sym
		{
			int temp_idx = file_index;
			switch (input[file_index])
			{
			case '.':
				list[lex_index++] = symboltoken('.');
			case '[':
				list[lex_index++] = symboltoken('[');
			case ']':
				list[lex_index++] = symboltoken(']');
			case ',':
				list[lex_index++] = symboltoken(',');
			case ';':
				list[lex_index++] = symboltoken(';');
			case ':':
				if (input[file_index + 1] == '=')
				{
					list[lex_index++] = symboltoken(":=");
				}
				else
				{
					list[lex_index++] symboltoken(':');
				}
			case '?':
				list[lex_index++] = symboltoken('?');
			case '(':
				list[lex_index++] = symboltoken('(');
			case ')':
				list[lex_index++] = symboltoken(')');
			case '=':
				if (input[file_index + 1] == '=')
				{
					list[lex_index++] = symboltoken('==');
				}
				else
				{
					printlexerror(4);
					return NULL;
				}
			case '<':
				if (input[file_index + 1] == '=')
				{
					list[lex_index++] = symboltoken('<=');
				}
				else if (input[file_index + 1] == '>')
				{
					list[lex_index++] = symboltoken('<>');
				}
				else
				{
					list[lex_index++] = symboltoken('<');
				}
			case '>':
				if (input[file_index + 1] == '=')
				{
					list[lex_index++] = symboltoken('>=');
				}
				else
				{
					list[lex_index++] = symboltoken('>');
				}
			case '%':
				list[lex_index++] = symboltoken('%');
			case '/':
				if (input[file_index + 1] == '/')
				{
					file_index++;
					while (input[file_index] != '\n')
					{
						file_index++;
					}
				}
				else
				{
					list[lex_index++] = symboltoken('/');
				}
			case '*':
				list[lex_index++] = symboltoken('*');
			case '-':
				list[lex_index++] = symboltoken('-');
			case '+':
				list[lex_index++] = symboltoken('+');
			default:
				printlexerror(4);
				return NULL;
			}

			file_index++;
		}
	}
	return list;
}

void reset_buf(char *buf, int *idx)
{
	buf[idx] = '\0';
	*idx = 0;
	strcpy(buf, "           ");
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
		case periodsym:
			printf("%11s\t%d", ".", periodsym);
			break;
		case varsym:
			printf("%11s\t%d", "var", varsym);
			break;
		case lbracketsym:
			printf("%11s\t%d", "[", lbracketsym);
			break;
		case procsym:
			printf("%11s\t%d", "procedure", procsym);
			break;
		case rbracketsym:
			printf("%11s\t%d", "]", rbracketsym);
			break;
		case callsym:
			printf("%11s\t%d", "call", callsym);
			break;
		case commasym:
			printf("%11s\t%d", ",", commasym);
			break;
		case beginsym:
			printf("%11s\t%d", "begin", beginsym);
			break;
		case semicolonsym:
			printf("%11s\t%d", ";", semicolonsym);
			break;
		case endsym:
			printf("%11s\t%d", "end", endsym);
			break;
		case assignsym:
			printf("%11s\t%d", ":=", assignsym);
			break;
		case ifsym:
			printf("%11s\t%d", "if", ifsym);
			break;
		case questionsym:
			printf("%11s\t%d", "?", questionsym);
			break;
		case dosym:
			printf("%11s\t%d", "do", dosym);
			break;
		case colonsym:
			printf("%11s\t%d", ":", colonsym);
			break;
		case readsym:
			printf("%11s\t%d", "read", readsym);
			break;
		case lparenthesissym:
			printf("%11s\t%d", "(", lparenthesissym);
			break;
		case writesym:
			printf("%11s\t%d", "write", writesym);
			break;
		case rparenthesissym:
			printf("%11s\t%d", ")", rparenthesissym);
			break;
		case identsym:
			printf("%11s\t%d", list[i].name, identsym);
			break;
		case eqlsym:
			printf("%11s\t%d", "==", eqlsym);
			break;
		case numbersym:
			printf("%11d\t%d", list[i].value, numbersym);
			break;
		case neqsym:
			printf("%11s\t%d", "<>", neqsym);
			break;
		case modsym:
			printf("%11s\t%d", "%", modsym);
			break;
		case lsssym:
			printf("%11s\t%d", "<", lsssym);
			break;
		case divsym:
			printf("%11s\t%d", "/", divsym);
			break;
		case leqsym:
			printf("%11s\t%d", "<=", leqsym);
			break;
		case multsym:
			printf("%11s\t%d", "*", multsym);
			break;
		case gtrsym:
			printf("%11s\t%d", ">", gtrsym);
			break;
		case subsym:
			printf("%11s\t%d", "-", subsym);
			break;
		case geqsym:
			printf("%11s\t%d", ">=", geqsym);
			break;
		case addsym:
			printf("%11s\t%d", "+", addsym);
			break;
		case whilesym:
			printf("%11s\t%d", "while", whilesym);
			break;
		default:
			printf("%11s\t%s", "err", "err");
			break;
		}
		printf("\n");
	}
	printf("\n");
}
//////////////////////////////////////////////////////////////////////
void printlexerror(int type) // done
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Number Length\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Identifier Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");

	free(list);
	return;
}
//////////////////////////////////////////////////////////////////////