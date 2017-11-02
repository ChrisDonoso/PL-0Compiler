#ifndef _definitions_h
#define _definitions_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 32768
#define MAX_LEXI_LEVELS 3
#define MAX_NUMBER_LENGTH 5
#define MAX_IDENTIFIER_LENGTH 11
#define MAX_SYMBOL_TABLE_SIZE 1000

typedef struct stack
{
	int *array;
	int capacity;
} stack;

typedef struct instruction
{
	int op;
	int l;
	int m;
} Instruction;

typedef struct node
{
	char *data;
	char name[21];
	int value;
	struct node *next;
} Node;

typedef struct symbol
{
	int kind;
	char name[21];
	int val;
	int level;
	int addr;
	char procedureIdentity[MAX_IDENTIFIER_LENGTH + 1];

} Symbol;

Symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
Symbol nameValue[MAX_SYMBOL_TABLE_SIZE];
Instruction code[MAX_CODE_LENGTH];

Node *prog;


long int inputCharsSize = 0;
int ip = 0, bp = 0;
int inVm = 0, outVm = 0;
int vmActivate = 0, lexActivate = 0, parserActivate = 0;

int nulsym = 1, identsym = 2, numbersym = 3, plussym = 4,
minussym = 5, multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9,
neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14,
lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
periodsym = 19, becomesym = 20, beginsym = 21, endsym = 22, ifsym = 23,
thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33;

int LIT = 1, OPR = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, 
JMP = 7, JPC = 8 , SIO = 9, SIO2 = 10, SIO3 = 11;

int tokenCount = 0, symbolTableIndex = 0, codeIndex = 0, 
nameValueIndex = 0, tempIndex = 0, level = 0;

char procedureList[MAX_LEXI_LEVELS][MAX_IDENTIFIER_LENGTH + 1];

char *symbolicRepresentation[] = {"nulsym", "identsym", "numbersym",
"plussym", "minussym", "multsym", " slashsym", "oddsym", "eqlsym",
"neqsym", "lessym", "leqsym", "gtrsym", "geqsym", "lparentsym",
"rparentsym", "commasym", "semicolonsym", "periodsym", "becomesym",
"beginsym", "endsym", "ifsym", "thensym", "whilesym", "dosym", 
"callsym","constsym", "varsym", "procsym", "writesym", "readsym"};

char IRMapping[34][64] = 
{
	"ZERO",
	"nulsym",
	"identsym",
	"numbersym",
	"plussym",
	"minussym",
	"multsym",
	"slashsym",
	"oddsym",
	"eqlsym",
	"neqsym",
	"lessym",
	"leqsym",
	"gtrsym",
	"geqsym",
	"lparentsym",
	"rparentsym",
	"commasym",
	"semicolonsym",
	"periodsym",
	"becomesym",
	"beginsym",
	"endsym",
	"ifsym",
	"thensym",
	"whilesym",
	"dosym",
	"callsym",
	"constsym",
	"varsym",
	"procsym",
	"writesym",
	"readsym",
	"elsesym",
};

char symbols[] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '<', '>', ';', ':'};

char reserved[15][32] = 
{
	"const",
	"var",
	"procedure",
	"call",
	"begin",
	"end",
	"if",
	"then",
	"else",
	"while",
	"do",
	"read",
	"write",
	"odd",
};

char inputChars[MAX_CODE_LENGTH];
char lexemeTable[MAX_CODE_LENGTH];
char lexemeList[MAX_CODE_LENGTH];
char symbolicLexemeList[MAX_CODE_LENGTH];

char buffer[21];


//VM functions.
int base(int l, int base, stack *s);
void isaPrint(int op);

//Parser functions.
Node *block(Node *ptr);
void codeGeneration(int op, int l, int m);
Node *condition(Node *ptr);
Node *destroyNodes(Node *ptr);
void error(int errorNumber);
Symbol findSymbol(Node *ptr);
Node *expression(Node *ptr);
Node *factor(Node *ptr);
Node *openFile(char *inputFile);
void printCode();
void printCodeScreen();
void printListRecursive(Node *ptr);
void printListRecursiveHelper(Node *ptr);
void printSymbolTable();
void printSymbolicRepresentation(Node *tokenList);
void program(Node *ptr);
Node *term(Node *ptr);
Node *statement(Node *ptr);
Node *storeTokenList(char *inputFile);

//Lexical Analyzer functions.
int reservedIndex(char * identifier);
int mapReserved(int spotInReserved);
int mapSymbol(char * symbol);
int isUpper(char theChar);
int isLower(char theChar);
int isAlpha(char theChar);
int isDigit(char theChar);
int isAlphanumeric(char theChar);
int isInvisible(char theChar);
int isSymbol(char theChar);
int isValid(char theChar);
void throwError(char * message);
char getChar(int ignoreValidity);
void ungetChar();
void clearBuffer();
void addToBuffer(char theChar);
void openFiles(char * inputFile, char * outputFile);
void clearLexemeOutput();
void insertToLexemeTable(char * lexeme, int tokenType);
void insertIntToLexemeList(int num);
void insertStrToLexemeList(char * identifier);
void processIdentifier(char * identifier);
void processNumber(char * num);
void processSymbol(char * sym);
void processText();
void echoInput();




#endif
