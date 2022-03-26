

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

int alphatoken(char *input, int idx);
int numbertoken(char *input, int idx);
int symboltoken(char *input, int idx);
int isReserved(char *name);
void printlexerror(int type);
void printtokens();

lexeme *lexanalyzer(char *input, int printFlag)
{

	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	lex_index = 0;

	int input_len = strlen(input);
	int input_idx = 0;

	char in;

	while (input_idx < input_len)
	{
		in = input[input_idx];

		if (isspace(in) || iscntrl(in))
		{
			input_idx++;
		}
		else if (isalpha(in))
		{
			input_idx = alphatoken(input, input_idx);

			// ident len > 11
			if (input_idx == -1)
			{
				printlexerror(3);
				return NULL;
			}
		}
		else if (isdigit(in))
		{
			input_idx = numbertoken(input, input_idx);

			// Num len > 5
			if (input_idx == -1)
			{
				printlexerror(1);
				return NULL;
			}
			// ident starts w num
			else if (input_idx == -2)
			{
				printlexerror(2);
				return NULL;
			}
		}
		else // symbol
		{
			input_idx = symboltoken(input, input_idx);

			if (input_idx == -1)
			{
				printlexerror(4);
				return NULL;
			}
		}
	}

	if (printFlag)
		printtokens();
	list[lex_index++].type = -1;

	return list;
}

int alphatoken(char *input, int idx)
{
	char *buf = malloc(sizeof(char) * MAX_IDENT_LEN);
	int buf_idx = 0;

	buf[buf_idx++] = input[idx++];

	while (
		buf_idx < MAX_IDENT_LEN &&
		(isalpha(input[idx]) || isdigit(input[idx])))
	{
		buf[buf_idx++] = input[idx++];
	}

	if (buf_idx > MAX_IDENT_LEN)
	{
		// ident len error
		free(buf);
		return -1;
	}
	else // broke on space/cntrl/sym
	{
		if (!isReserved(buf))
		{
			// Add non-reserved word to lexeme list as valid identifier
			strcpy(list[lex_index].name, buf);
			list[lex_index].value = identsym;
			list[lex_index].type = identsym;
		}
		lex_index++;
	}

	// return to next index
	free(buf);
	return idx;
}

int numbertoken(char *input, int idx)
{
	char *buf = malloc(sizeof(char) * MAX_NUMBER_LEN);
	int buf_idx = 0;

	buf[buf_idx++] = input[idx++];

	while (
		buf_idx < MAX_NUMBER_LEN &&
		isdigit(input[idx]))
	{
		buf[buf_idx++] = input[idx++];
	}

	if (buf_idx >= MAX_NUMBER_LEN)
	{
		// invalid num len error
		free(buf);
		return -2;
	}
	else if (isalpha(input[idx]))
	{
		// ident starts with num error
		free(buf);
		return -1;
	}
	else // space/cntrl/sym break valid, e.g. 'end of token'
	{
		strcpy(list[lex_index].name, "numbersym");
		list[lex_index].value = atoi(buf);
		list[lex_index].type = numbersym;
		lex_index++;
	}

	// return to next index
	free(buf);
	return idx;
}

int symboltoken(char *input, int idx)
{
	int comment = 0;
	switch (input[idx])
	{
	case '.':
		strcpy(list[lex_index].name, ".");
		list[lex_index].value = periodsym;
		list[lex_index].type = periodsym;
		break;
	case '[':
		strcpy(list[lex_index].name, "[");
		list[lex_index].value = lbracketsym;
		list[lex_index].type = lbracketsym;
		break;
	case ']':
		strcpy(list[lex_index].name, "]");
		list[lex_index].value = rbracketsym;
		list[lex_index].type = rbracketsym;
		break;
	case ',':
		strcpy(list[lex_index].name, ",");
		list[lex_index].value = commasym;
		list[lex_index].type = commasym;
		break;
	case ';':
		strcpy(list[lex_index].name, ";");
		list[lex_index].value = semicolonsym;
		list[lex_index].type = semicolonsym;
		break;
	case ':':
		if (input[idx + 1] == '=')
		{
			strcpy(list[lex_index].name, ":=");
			list[lex_index].value = assignsym;
			list[lex_index].type = assignsym;
			idx++;
		}
		else
		{
			strcpy(list[lex_index].name, ":");
			list[lex_index].value = colonsym;
			list[lex_index].type = colonsym;
		}

		break;
	case '?':
		strcpy(list[lex_index].name, "?");
		list[lex_index].value = questionsym;
		list[lex_index].type = questionsym;
		break;
	case '(':
		strcpy(list[lex_index].name, "(");
		list[lex_index].value = lparenthesissym;
		list[lex_index].type = lparenthesissym;
		break;
	case ')':
		strcpy(list[lex_index].name, ")");
		list[lex_index].value = rparenthesissym;
		list[lex_index].type = rparenthesissym;
		break;
	case '<':
		if (input[idx + 1] == '>')
		{
			strcpy(list[lex_index].name, "<>");
			list[lex_index].value = neqsym;
			list[lex_index].type = neqsym;
			idx++;
		}
		else if (input[idx + 1] == '=')
		{
			strcpy(list[lex_index].name, "<=");
			list[lex_index].value = leqsym;
			list[lex_index].type = leqsym;
			idx++;
		}
		else
		{
			strcpy(list[lex_index].name, "<");
			list[lex_index].value = lsssym;
			list[lex_index].type = lsssym;
		}

		break;
	case '>':
		if (input[idx + 1] == '=')
		{
			strcpy(list[lex_index].name, ">=");
			list[lex_index].value = geqsym;
			list[lex_index].type = geqsym;
			idx++;
		}
		else
		{
			strcpy(list[lex_index].name, ">");
			list[lex_index].value = gtrsym;
			list[lex_index].type = gtrsym;
		}

		break;
	case '%':
		strcpy(list[lex_index].name, "%");
		list[lex_index].value = modsym;
		list[lex_index].type = modsym;
		break;
	case '/':
		if (input[idx + 1] == '/')
		{
			comment = 1;
			while (
				!(input[idx + 1] == '\r' ||
				  input[idx + 1] == '\n' ||
				  input[idx + 1] == '\0'))
			{
				idx++;
			}
		}
		else
		{
			strcpy(list[lex_index].name, "/");
			list[lex_index].value = divsym;
			list[lex_index].type = divsym;
		}

		break;
	case '*':
		strcpy(list[lex_index].name, "*");
		list[lex_index].value = multsym;
		list[lex_index].type = multsym;
		break;
	case '-':
		strcpy(list[lex_index].name, "-");
		list[lex_index].value = subsym;
		list[lex_index].type = subsym;
		break;
	case '+':
		strcpy(list[lex_index].name, "+");
		list[lex_index].value = addsym;
		list[lex_index].type = addsym;
		break;
	case '=':
		if (input[idx + 1] == '=')
		{
			strcpy(list[lex_index].name, "==");
			list[lex_index].value = eqlsym;
			list[lex_index].type = eqlsym;
			idx++;
		}
		else
		{
			return -1;
		}
		break;
	default:
		return -1;
	}

	if (!comment)
	{
		lex_index++;
	}

	// return to next index
	return idx + 1;
}

int isReserved(char *name)
{
	token_type type;

	if (strcmp(name, "var") == 0)
		type = varsym;
	else if (strcmp(name, "procedure") == 0)
		type = procsym;
	else if (strcmp(name, "call") == 0)
		type = callsym;
	else if (strcmp(name, "begin") == 0)
		type = beginsym;
	else if (strcmp(name, "end") == 0)
		type = endsym;
	else if (strcmp(name, "if") == 0)
		type = ifsym;
	else if (strcmp(name, "do") == 0)
		type = dosym;
	else if (strcmp(name, "read") == 0)
		type = readsym;
	else if (strcmp(name, "write") == 0)
		type = writesym;
	else if (strcmp(name, "while") == 0)
		type = whilesym;
	else
		return 0;

	// Add reserved word to lexeme list
	strcpy(list[lex_index].name, name);
	list[lex_index].value = type;
	list[lex_index].type = type;

	return 1;
}

void printlexerror(int type)
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
