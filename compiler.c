#include "definitions.h"

void vm(void);
void parser(void);
void lexicalAnalyzer(char *inputFile);
	
int main(int argc, char *argv[])
{
	int i = 0;
	char tempInput[21], input[21];

	printf("What is the name of the input file? ");
	scanf("%s", tempInput);

	strcpy(input, tempInput);

	// Print the list of lexemes/tokens (scanner output) to the screen
	lexicalAnalyzer(tempInput);	
	parser();
	vm();

	for(i = 0; i < argc - 1; i++)
	{
		if(strcmp(argv[i + 1], "-l") == 0)
			lexActivate = 1;

		if(strcmp(argv[i + 1], "-a") == 0)
			parserActivate = 1;

		if(strcmp(argv[i + 1], "-v") == 0)
			vmActivate = 1;
	}

	if(lexActivate)
	{
		ip = 0, inputCharsSize = 0, bp = 0;
		printf("\n");
		lexicalAnalyzer(input);
		printf("\n");
	}

	if(parserActivate)
		parser();

	if(vmActivate)
		vm();

	else if(lexActivate == 0 && parserActivate == 0 && vmActivate == 0)
	{
		vmActivate = 0;
		inVm = 1;
		outVm = 1;
		vm();
	}

	return 0;
}

/*
	Virtual Machine
*/

//Finds the base.
int base(int l, int base, stack *s)
{
	int b1;

	b1 = base;

	while(l > 0)
	{
		b1 = s->array[b1 + 1];
		l--;
	}

	return b1;
}

//Prints out ISA.
void isaPrint(int op)
{
	switch(op)
	{
		case 1:
			printf("lit");
			break;

		case 2:
			printf("opr");
			break;

		case 3:
			printf("lod");
			break;

		case 4:
			printf("sto");
			break;

		case 5:
			printf("cal");
			break;

		case 6:
			printf("inc");
			break;

		case 7:
			printf("jmp");
			break;

		case 8:
			printf("jpc");
			break;

		case 9:
			printf("sio");
			break;
	}
}

//Virtual Machine.
void vm(void)
{
	if(vmActivate)
	{
		printf("\nVIRTUAL MACHINE\n");
		printf("===============\n");		
	}


	int textCount = 0, i = 0, j = 0, run = 1, counter = 0, line = 0;
	int opCount = 0, lCount = 0, mCount = 0;
	int sp = 0, bp = 1, pc = 0;
	int *fetchArray, *opArray, *lArray, *mArray;
	char buffer[5];
	char *word;

	//Opens the text file.
	FILE *ifp = NULL;

	ifp = fopen("vmInput.txt", "r");


	//Acquires the number of instruction for array size.
	while (fscanf(ifp, "%s", buffer) != EOF)
		textCount++;

	//Closes text file.
	fclose(ifp);
	ifp = NULL;

	fetchArray = malloc(sizeof(int) * (textCount + 1));
	opArray = malloc(sizeof(int) * (textCount / 3) + 1);
	lArray = malloc(sizeof(int) * (textCount / 3) + 1);
	mArray = malloc(sizeof(int) * (textCount / 3) + 1);

	ifp = fopen("vmInput.txt", "r");


	//Stores the instruction in the array
	while (fscanf(ifp, "%s", buffer) != EOF)
	{
		word = malloc(sizeof(char) * strlen(buffer) + 1);

		strcpy(word, buffer);

		fetchArray[i++] = atoi(word);

		free(word);
	}

	fclose(ifp);
	ifp = NULL;

	//Creates and initializes stack struct.
	stack *s = malloc(sizeof(stack));
	s->capacity = MAX_STACK_HEIGHT;
	s->array = calloc(s->capacity, sizeof(int));

	//Creates and initializes instruction struct.
	Instruction *in = malloc(sizeof(Instruction));

	//Prints out program in interpreted assembly language.
	if(vmActivate)
		printf("Line   OP   L   M\n");

	for(i = 0; i < textCount; i++)
	{
		if(counter == 0)
		{	
			if(vmActivate)
			{
				printf(" %d    ", line++);
				isaPrint(fetchArray[i]);
			}
			opArray[opCount++] = fetchArray[i];
		}

		if(counter == 1)
		{
			if(vmActivate)
				printf("  %d", fetchArray[i]);

			lArray[lCount++] = fetchArray[i];
		}

		if(counter == 2)
		{	
			if(vmActivate)
				printf("   %d\n", fetchArray[i]);

			mArray[mCount++] = fetchArray[i];
		}

		counter++;

		if(counter > 2)
			counter = 0;
	}

	if(vmActivate)
	{
		printf("\n		pc bp sp  stack\n");
		printf("Initial Values	%d  %d  %d\n", pc, bp, sp);
	}

	//Executes the instructions.
	while(run)
	{
		in->op = opArray[pc];
		in->l = lArray[pc];
		in->m = mArray[pc];

		if(vmActivate)
		{
			printf("%d  ", pc);
			isaPrint(in->op);
		}

		pc++;
		switch(in->op)
		{
			//LIT
			case 1:
				sp = sp + 1;
				s->array[sp] = in->m;
				break;	

			//OPR
			case 2:
				if(in->m == 0)
				{
					sp = bp - 1;
					pc = s->array[sp + 4];
					bp = s->array[sp + 3];
				}

				//NEG
				if(in->m == 1)
					s->array[sp] = s->array[sp] * (-1);

				//ADD
				if(in->m == 2)
				{
					sp = sp - 1;
					s->array[sp] = s->array[sp] + s->array[sp + 1];
				}

				//SUB
				if(in->m == 3)
				{
					sp = sp - 1;
					s->array[sp] = s->array[sp] - s->array[sp + 1]; 
				}

				//MUL
				if(in->m == 4)
				{
					sp = sp - 1;
					s->array[sp] = s->array[sp] * s->array[sp + 1]; 				
				}

				//DIV
				if(in->m == 5)
				{
					sp = sp - 1;
					s->array[sp] = s->array[sp] / s->array[sp + 1]; 				
				}

				//ODD 
				if(in->m == 6)
				{
					s->array[sp] = s->array[sp] % 2;
				}

				//MOD
				if(in->m == 7)
				{
					sp = sp - 1;
					s->array[sp] = s->array[sp] % s->array[sp + 1];
				}

				//EQL
				if(in->m == 8)
				{
					sp = sp - 1;
					s->array[sp] = (s->array[sp] == s->array[sp + 1] ? 1 : 0);
				}

				//NEQ
				if(in->m == 9)
				{
					sp = sp - 1;
					s->array[sp] = (s->array[sp] != s->array[sp + 1] ? 1 : 0);
				}

				//LSS
				if(in->m == 10)
				{
					sp = sp - 1;
					s->array[sp] = (s->array[sp] < s->array[sp + 1] ? 1 : 0); 
				}

				//LEQ
				if(in->m == 11)
				{
					sp = sp - 1;
					s->array[sp] = (s->array[sp] <= s->array[sp + 1] ? 1 : 0);
				}

				//GTR
				if(in->m == 12)
				{
					sp = sp - 1;
					s->array[sp] = (s->array[sp] > s->array[sp + 1] ? 1 : 0);
				}

				//GEQ
				if(in->m == 13)
				{
					sp = sp - 1;
					s->array[sp] = (s->array[sp] >= s->array[sp + 1] ? 1 : 0);
				}
				break;

			//LOD
			case 3:
				sp = sp + 1;
				s->array[sp] = s->array[base(in->l,bp, s) + in->m];
				break;


			//STO
			case 4:
				s->array[base(in->l,bp, s) + in->m] = s->array[sp];
				sp = sp - 1;
				break;


			//CAL
			case 5:
				s->array[sp + 1] = 0;
				s->array[sp + 2] = base(in->l, bp, s);
				s->array[sp + 3] = bp;
				s->array[sp + 4] = pc;
				bp = sp + 1;
				pc = in->m;
				break;


			//INC
			case 6:
				sp = sp + in->m;
				break;

			//JMP
			case 7:
				pc = in->m;
				break;

			//JPC
			case 8:
				if(s->array[sp] == 0)
					pc = in->m;
				sp = sp - 1;
				break;

			//SIO
			case 9:
				if(in->m == 1)
				{
					if(outVm == 1)
					{
						printf("Output: %d\n", s->array[sp]);
					}

					sp = sp - 1;
				}

				if(in->m == 2)
				{
					sp = sp + 1;

					if(inVm == 1)
					{
						printf("Input: ");
						scanf("%d", &s->array[sp]);
					}
				}

				if(in->m == 3)
				{
					pc = 0, bp = 0, sp = 0;
					run = 0;
				}
				break;
		}

		//Prints out values and stack
		if(vmActivate)
		{
			printf("  %d   %d 	%d  %d  %d  ", in->l, in->m, pc, bp, sp);

			for(i = 1; i < sp + 1; i++)
			{
				printf(" %d", s->array[i]);

				if(sp > bp && bp > 1 && i == bp)
					printf("|");
			}

			if(in->op == 5)
			{
				printf("|");

				for(j = 0; j < 4; j++)
				{
					printf(" %d", s->array[bp + j]);
				}
			}

			printf("\n");
		}
	}

	free(s->array);
	free(s);
	free(in);
	free(fetchArray);
	free(opArray);
	free(lArray);
	free(mArray);
}

/*
	Parser
*/

void parser(void)
{

	if(parserActivate)
	{
		printf("\n\nPARSER\n");
		printf("=======\n");
	}
	Node *ptr = NULL, *tokenList = NULL;

	//Opens the file and stores the tokens in a linked list.
	tokenList = storeTokenList("lexTokens.txt");

	//Prints out the tokens.
	printListRecursive(tokenList);
	// printSymbolicRepresentation(tokenList);

	//Acquires tokens and removes the symbols.
	ptr = openFile("lexTokens.txt");
	
	//Runs the parser.
	program(ptr);

	//Prints out assembly code and symbol table.
	// printCode();
	printCode();

	if(parserActivate)
	{
		printf("\n");
		printCodeScreen();
	}

	//Uncomment to print out symbol table to command terminal.
	// printSymbolTable();

	//Frees nodes.
	destroyNodes(tokenList);
	destroyNodes(ptr);

}

//Creates nodes.
Node *createNode(char *word)
{
	Node *ptr = malloc(sizeof(Node));
	ptr->data = malloc(sizeof(char) * (strlen(word) + 1));

	if (ptr == NULL)
	{
		fprintf(stderr, "Error: Out of memory in createNode()\n");
		exit(1);
	}

	strcpy(ptr->data, word);
	ptr->value = 1000;
	ptr->next = NULL;

	return ptr;
}

//Inserts tokens into nodes.
Node *insertNode(Node *ptr, char *word)
{
	Node *orginalptr = ptr;

	if(ptr == NULL)
		return createNode(word);

	while(ptr->next != NULL)
		ptr = ptr->next;

	ptr->next = createNode(word);

	return orginalptr;
}

//Destroy nodes.
Node *destroyNodes(Node *ptr)
{
	if(ptr == NULL)
		return NULL;

	destroyNodes(ptr->next);

	free(ptr);

	return NULL;
}

//Opens the text files and stores the tokens without symbols.
Node *openFile(char *inputFile)
{
	FILE *ifp = fopen(inputFile, "r");
	Node *ptr = NULL;

	int symbolActivate = 0;
	char buffer[801];
	char *word;

	//Checks if text files exists.
	if(ifp == NULL)
	{
		fprintf(stderr, "Could not open %s in main()!\n", inputFile);
		exit(0);		
	}

	//Goes through the text file to acquire the lexeme tokens.
	while(fscanf(ifp, "%s", buffer) != EOF)
	{
		word = malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(word, buffer);

		if(symbolActivate)
		{			
			symbolActivate = 0;	

			strcpy(nameValue[nameValueIndex++].name, word);
		}
		
		//Stores the name and values of the symbols following numbersym and varsym.
		else if(strcmp(word, "2") == 0 || strcmp(word, "3") == 0)
		{
			ptr = insertNode(ptr, word);
			symbolActivate = 1;

			if(strcmp(word, "2") == 0)
				nameValue[nameValueIndex].kind = 2;

			else 
				nameValue[nameValueIndex].kind = 3;
		}

		else 
			ptr = insertNode(ptr, word);

		free(word);
	}

	//Adds null in order to check last token.
	//If null not included, segmentation fault would occur.
	ptr = insertNode(ptr, "NULL");

	word = NULL;
	fclose(ifp);
	ifp = NULL;

	return ptr;
}

//Opens the text files and stores all of the tokens and symbols.
Node *storeTokenList(char *inputFile)
{
	FILE *ifp = fopen(inputFile, "r");
	Node *list = NULL;

	char buffer[801];
	char *word;

	//Checks if text files exists.
	if(ifp == NULL)
	{
		fprintf(stderr, "Could not open %s in main()!\n", inputFile);
		exit(0);		
	}

	//Goes through the text file to acquire the lexeme tokens.
	while(fscanf(ifp, "%s", buffer) != EOF)
	{
		word = malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(word, buffer);

		list = insertNode(list, word);

		free(word);
	}

	word = NULL;
	fclose(ifp);
	ifp = NULL;

	return list;
}

//Stores assembly code into an array.
void codeGeneration(int op, int l, int m)
{
	code[codeIndex].op = op;
	code[codeIndex].l = l;
	code[codeIndex].m = m;

	codeIndex++;
}

//Prints out the error statements.
void error(int errorNumber)
{
	switch(errorNumber)
	{
		case 1:
			printf("Error number 1: Use = instead of :=.\n");
			break;

		case 2:
			printf("Error number 2: = must be followed by a number.\n");
			break;

		case 3:
			printf("Error number 3: Identifier must be followed by =.\n");
			break;

		case 4:
			printf("Error number 4: const, var, procedure must be followed by an identifier.\n");
			break;

		case 5:
			printf("Error number 5: Semicolon or comma missing.\n");
			break;

		case 6:
			printf("Error number 6: Incorrect symbol after procedure declaration.\n");
			break;

		case 7:
			printf("Error number 7: Statement expected.\n");
			break;

		case 8:
			printf("Error number 8: Incorrect symbol after statement part in block\n");
			break;

		case 9:
			printf("Error number 9: Period expected.\n");
			break;

		case 10:
			printf("Error number 10: Semicolon between statements missing.\n");
			break;

		case 11:
			printf("Error number 11: Undeclared identifier.\n");
			break;

		case 12:
			printf("Error number 12: Assignment operator expected.\n");
			break;

		case 13:
			printf("Error number 13: Assignment operator expected.\n");
			break;

		case 14:
			printf("Error number 14: call must be followed by an identifier.\n");
			break;

		case 15:
			printf("Error number 15: Call of a constant or variable is meaningless.\n");
			break;

		case 16:
			printf("Error number 16: then expected.\n");
			break;

		case 17:
			printf("Error number 17: Semicolon or } expected.\n");
			break;

		case 18:
			printf("Error number 18: do expected\n");
			break;

		case 19:
			printf("Error number 19: Incorrect symbol following statement.\n");
			break;

		case 20:
			printf("Error number 20: Relational operator expected.\n");
			break;

		case 21:
			printf("Error number 21: Expression must not contain a procedure identifier.\n");
			break;

		case 22:
			printf("Error number 22: Right parenthesis missing.\n");
			break;

		case 23:
			printf("Error number 23: The preceding factor cannot begin with this symbol.\n");
			break;

		case 24:
			printf("Error number 24: An expression cannot begin with this symbol.\n");
			break;

		case 25:
			printf("Error number 25: This number is too large.\n");
			break;

		case 26:
			printf("Error number 26: Read/Write should be followed by an identifier.\n");
			break;
	}

	exit(1);	
}

//Finds the necessary symbol from the symbol table.
Symbol findSymbol(Node *ptr)
{
	for(int i = symbolTableIndex - 1; i >= 0; i--)
	{	
		if(symbolTable[i].level <= level)
		{		
			if(strcmp(ptr->name, symbolTable[i].name) == 0) 
				return symbolTable[i];
		}
	}

	return symbolTable[symbolTableIndex];
}

//Outputs assembly code to a text file.
void printCode()
{
	FILE *ofp = NULL;
	int i;

	ofp = fopen("vmInput.txt", "w");

	for(i = 0; i < codeIndex; i++)
	{
		fprintf(ofp, "%d ", code[i].op);
		fprintf(ofp, "%d ", code[i].l);
		fprintf(ofp, "%d\n", code[i].m);	
	}

	fclose(ofp);
}

//Outputs assembly code to the s.
void printCodeScreen()
{
	int i;

	for(i = 0; i < codeIndex; i++)
	{
		printf("%d ", code[i].op);
		printf("%d ", code[i].l);
		printf("%d\n", code[i].m);	
	}
}

//Prints out tokens.
void printListRecursive(Node *ptr)
{
	if (ptr == NULL)
	{
		printf("(empty list)\n");
		return;
	}

	printListRecursiveHelper(ptr);
}

void printListRecursiveHelper(Node *ptr)
{
	if (ptr == NULL)
		return;

	printf("%s%c", ptr->data, (ptr->next == NULL) ? '\n' : ' ');
	printListRecursiveHelper(ptr->next);
}

//Prints symbol table.
void printSymbolTable()
{
	printf("\nKind	Name		Value	L	M\n");

	for(int i = 0; i < symbolTableIndex; i++)
	{	
		if(symbolTable[i].kind == 1)
		{
			printf("%d", symbolTable[i].kind);
			printf("\t%s	", symbolTable[i].name);
			printf("\t%d", symbolTable[i].val);
			printf("\t%d", symbolTable[i].level);
			printf("\t%d\n", symbolTable[i].addr);
		}

		if(symbolTable[i].kind == 2)
		{
			printf("%d", symbolTable[i].kind);
			printf("\t%s	", symbolTable[i].name);
			printf("\t%d", symbolTable[i].val);
			printf("\t%d", symbolTable[i].level);
			printf("\t%d\n", symbolTable[i].addr);
		}		

		if(symbolTable[i].kind == 3)
		{
			printf("%d", symbolTable[i].kind);
			printf("\t%s	", symbolTable[i].name);
			printf("\t%d", symbolTable[i].val);
			printf("\t%d", symbolTable[i].level);
			printf("\t%d\n", symbolTable[i].addr);
		}			
	}
}

//Prints out symbolic representation of tokens.
void printSymbolicRepresentation(Node *tokenList)
{
	int symbolActivate = 0;

	printf("\n");

	while(tokenList != NULL)
	{
		if(symbolActivate)
		{
			printf("%s ", tokenList->data);
			tokenList = tokenList->next;
			symbolActivate = 0;				
		}

		else if(strcmp(tokenList->data, "2") == 0 || strcmp(tokenList->data, "3") == 0)
		{
			printf("%s ", symbolicRepresentation[atoi(tokenList->data) - 1]);
			tokenList = tokenList->next;
			symbolActivate = 1;
		}

		else 
		{
			printf("%s ", symbolicRepresentation[atoi(tokenList->data) - 1]);
			tokenList = tokenList->next;
		}
	}

	printf("\n\n");
}

void program(Node *ptr)
{
	ptr = block(ptr);

	if(atoi(ptr->data) != periodsym)
		error(9);

	if(parserActivate)
		printf("No errors, program is syntactially correct!\n");

	codeGeneration(SIO, 0 , 3);
}

Node *block(Node *ptr)
{
	int space = 0, varCount = 4, jmpIndex = codeIndex;

	codeGeneration(JMP, 0 , 0);

	//Store kind, name, and value into symbol table.
	if(atoi(ptr->data) == constsym)
	{
		do
		{
			ptr = ptr->next;

			if(atoi(ptr->data) != identsym)
				error(4);

			ptr = ptr->next;

			if(atoi(ptr->data) != eqlsym)
				error(3);

			ptr = ptr->next;

			if(atoi(ptr->data) != numbersym)
				error(2);

			strcpy(ptr->name, nameValue[tempIndex].name);

			symbolTable[symbolTableIndex].kind = 1;
			strcpy(symbolTable[symbolTableIndex].name, nameValue[tempIndex++].name);
			symbolTable[symbolTableIndex].level = level;
			symbolTable[symbolTableIndex].val = atoi(nameValue[tempIndex++].name);
			ptr->value = symbolTable[symbolTableIndex++].val;

			ptr = ptr->next;

		}	while(atoi(ptr->data) == commasym);

		if(atoi(ptr->data) != semicolonsym)
			error(10);

		ptr = ptr->next;
	}

	//Store kind, name, L, and M into symbol table.
	if(atoi(ptr->data) == varsym)
	{
		varCount += space;

		do
		{
			ptr = ptr->next;

			if(atoi(ptr->data) != identsym)
				error(4);
		
			strcpy(ptr->name, nameValue[tempIndex].name);

			symbolTable[symbolTableIndex].kind = 2;
			strcpy(symbolTable[symbolTableIndex].name, nameValue[tempIndex++].name);
			symbolTable[symbolTableIndex].level = level;
			symbolTable[symbolTableIndex++].addr = varCount;

			ptr = ptr->next;

			varCount++;
		}	while(atoi(ptr->data) == commasym);

		if(atoi(ptr->data) != semicolonsym)
			error(10);

		ptr = ptr->next;
	}

	//Store kind, name, L and M into symbol table.
	if(atoi(ptr->data) == procsym)
	{	
		ptr = ptr->next;

		if(atoi(ptr->data) != identsym)
			error(4);

		strcpy(procedureList[level], nameValue[tempIndex].name);
		strcpy(ptr->name, nameValue[tempIndex].name);

		symbolTable[symbolTableIndex].kind = 3;
		strcpy(symbolTable[symbolTableIndex].name, nameValue[tempIndex++].name);
		symbolTable[symbolTableIndex].addr = jmpIndex + 1;
		symbolTable[symbolTableIndex - 1].level = level;

		++level;
		symbolTableIndex++;
		ptr = ptr->next;

		if(atoi(ptr->data) != semicolonsym)
			error(6);

		ptr = ptr->next;

		ptr = block(ptr);

		if(atoi(ptr->data) != semicolonsym)
			error(10);


		codeGeneration(OPR, 0, 0);

		ptr = ptr->next;

		--level;
	}

	code[jmpIndex].m = codeIndex;

	codeGeneration(INC, 0, varCount);

	ptr = statement(ptr);

	return ptr;
}

Node *statement(Node *ptr)
{
	if(atoi(ptr->data) == identsym)
	{
		strcpy(ptr->name, nameValue[tempIndex++].name);

		Symbol tempSymbol = findSymbol(ptr);

		if(tempSymbol.kind != 2)
			error(12);
		
		ptr = ptr->next;

		if(atoi(ptr->data) != becomesym)
			error(1);

		ptr = ptr->next;

		ptr = expression(ptr);

		codeGeneration(STO, level - tempSymbol.level, tempSymbol.addr);
	}

	//Test call statement.
	else if(atoi(ptr->data) == callsym)
	{		
		ptr = ptr->next;
		strcpy(ptr->name, nameValue[tempIndex++].name);

		Symbol tempSymbol = findSymbol(ptr);

		if(atoi(ptr->data) != identsym)
			error(14);

		if(tempSymbol.kind != 3)
			error(12);		

		codeGeneration(CAL, level - tempSymbol.level, tempSymbol.addr);

		ptr = ptr->next;
	}

	else if(atoi(ptr->data) == beginsym)
	{
		ptr = ptr->next;

		ptr = statement(ptr);

		while(atoi(ptr->data) == semicolonsym)
		{
			ptr = ptr->next;

			ptr = statement(ptr);
		}

		if(atoi(ptr->data) != endsym)
			error(8);

		ptr = ptr->next;
	}

	//Test else statement.
	else if(atoi(ptr->data) == ifsym)
	{
		ptr = ptr->next;

		ptr = condition(ptr);

		int ifIndex = codeIndex;

		if(atoi(ptr->data) != thensym)
			error(16);

		codeGeneration(JPC, 0, 0);

		ptr = ptr->next;

		ptr = statement(ptr);

		int elseIndex = codeIndex;

		codeGeneration(JMP, 0, 0);

	
		code[ifIndex].m = codeIndex;

		if(atoi(ptr->data) == elsesym)
		{
			ptr = ptr->next;

			ptr = statement(ptr);
		}

		code[elseIndex].m = codeIndex;
	}

	else if(atoi(ptr->data) == whilesym)
	{
		int whileIndex = codeIndex, whileTwoIndex = 0;

		ptr = ptr->next;

		ptr = condition(ptr);

		whileTwoIndex = codeIndex;

		codeGeneration(JPC, 0, 0);

		if(atoi(ptr->data) != dosym)
			error(18);

		ptr = ptr->next;

		ptr = statement(ptr);

		codeGeneration(JMP, 0, whileIndex);

		code[whileTwoIndex].m = codeIndex;
	}

	else if(atoi(ptr->data) == readsym)
	{
		ptr = ptr->next;

		strcpy(ptr->name, nameValue[tempIndex++].name);

		Symbol tempSymbol = findSymbol(ptr);

		if(tempSymbol.kind != 2)
			error(11);

		codeGeneration(SIO, 0 , 2);
		codeGeneration(STO, level - tempSymbol.level, tempSymbol.addr);

		ptr = ptr->next;
	}

	else if(atoi(ptr->data) == writesym)
	{
		strcpy(ptr->name, nameValue[tempIndex++].name);

		Symbol tempSymbol = findSymbol(ptr);

		ptr = ptr->next;

		if(atoi(ptr->data) != identsym)
			error(26);

		if(tempSymbol.kind == 2)
			codeGeneration(LOD, level - tempSymbol.level, tempSymbol.addr);

		if(tempSymbol.kind == 1)
			codeGeneration(LIT, 0, tempSymbol.val);

		codeGeneration(SIO, 0, 1);

		ptr = ptr->next;
	}

	return ptr;
}

Node *condition(Node *ptr)
{
	if(atoi(ptr->data) == oddsym)
	{
		codeGeneration(OPR, 0, 6);

		ptr = ptr->next;

		ptr = expression(ptr);
	}

	else
	{
		int type = 0;

		ptr = expression(ptr);

		type = atoi(ptr->data) - 1;

		if((atoi(ptr->data) >= eqlsym && atoi(ptr->data) <= geqsym) == 0)
			error(20);

		ptr = ptr->next;

		ptr = expression(ptr);

		codeGeneration(OPR, 0, type);
	}

	return ptr;
}

Node *expression(Node *ptr)
{
	int op;

	if(atoi(ptr->data) == plussym || atoi(ptr->data) == minussym)
	{
		op = atoi(ptr->data);

		ptr = ptr->next;

		if(op == minussym)
			codeGeneration(OPR, 0, 1);
	}

	ptr = term(ptr);

	while(atoi(ptr->data) == plussym || atoi(ptr->data) == minussym)
	{
		op = atoi(ptr->data);

		ptr = ptr->next;

		ptr = term(ptr);

		if(op == plussym)
			codeGeneration(OPR, 0, 2);

		if(op == minussym)
			codeGeneration(OPR, 0, 3);
	}

	return ptr;
}

Node *term(Node *ptr)
{
	int op = 0;

	ptr = factor(ptr);

	while(atoi(ptr->data) == multsym || atoi(ptr->data) == slashsym)
	{
		op = atoi(ptr->data);

		ptr = ptr->next;

		ptr = factor(ptr);

		if(op == multsym)
			codeGeneration(OPR, 0, 4);

		else
			codeGeneration(OPR, 0, 5);
	}

	return ptr;
}

Node *factor(Node *ptr)
{
	if(atoi(ptr->data) == identsym)
	{
		strcpy(ptr->name, nameValue[tempIndex++].name);

		Symbol tempSymbol = findSymbol(ptr);

		ptr = ptr->next;

		if(tempSymbol.kind == 2)
			codeGeneration(LOD, level - tempSymbol.level, tempSymbol.addr);

		else if(tempSymbol.kind == 1)
			codeGeneration(LIT, 0, tempSymbol.val);
	}

	else if(atoi(ptr->data) == numbersym)
	{	
		codeGeneration(LIT, 0, atoi(nameValue[tempIndex++].name));

		ptr = ptr->next;
	}

	else if(atoi(ptr->data) == lparentsym)
	{
		ptr = ptr->next;

		ptr = expression(ptr);

		if(atoi(ptr->data) != rparentsym)
			error(22);

		ptr = ptr->next;
	}

	else
		error(24);

	return ptr;
}

/*
	Lexical Analyzer
*/

//Returns the index in reserved of the string pointed to by [identifier].
int reservedIndex(char * identifier)
{
	for(int i = 0; i < 14; i++)
	{
		if (strcmp(reserved[i], identifier) == 0)
		{
			return i;
		}
	}
	return -1;
}

//Maps an index in reserved to the proper internal representation...
int mapReserved(int spotInReserved)
{
	if (spotInReserved == 0)
		return constsym;
	if (spotInReserved == 1)
		return varsym;
	if (spotInReserved == 2)
		return procsym;
	if (spotInReserved == 3)
		return callsym;
	if (spotInReserved == 4)
		return beginsym;
	if (spotInReserved == 5)
		return endsym;
	if (spotInReserved == 6)
		return ifsym;
	if (spotInReserved == 7)
		return thensym;
	if (spotInReserved == 8)
		return elsesym;	
	if (spotInReserved == 9)
		return whilesym;
	if (spotInReserved == 10)
		return dosym;
	if (spotInReserved == 11)
		return readsym;
	if (spotInReserved == 12)
		return writesym;
	if (spotInReserved == 13)
		return oddsym;
    return -1;
}

//Maps a symbol from character to internal representation...
int mapSymbol(char * symbol)
{
	if (strcmp(symbol, "+") == 0)
		return plussym;
	if (strcmp(symbol, "-") == 0)
		return minussym;
	if (strcmp(symbol, "*") == 0)
		return multsym;
	if (strcmp(symbol, "/") == 0)
		return slashsym;
	if (strcmp(symbol, "(") == 0)
		return lparentsym;
	if (strcmp(symbol, ")") == 0)
		return rparentsym;
	if (strcmp(symbol, "=") == 0)
		return eqlsym;
	if (strcmp(symbol, ",") == 0)
		return commasym;
	if (strcmp(symbol, ".") == 0)
		return periodsym;
	if (strcmp(symbol, "<") == 0)
		return lessym;
	if (strcmp(symbol, ">") == 0)
		return gtrsym;
	if (strcmp(symbol, ";") == 0)
		return semicolonsym;
	if (strcmp(symbol, "<>") == 0)
		return neqsym;
	if (strcmp(symbol, "<=") == 0)
		return leqsym;
	if (strcmp(symbol, ">=") == 0)
		return geqsym;
	if (strcmp(symbol, ":=") == 0)
		return becomesym;
    return -1;
}

//Returns 1 iff [theChar] is a uppercase letter, 0 otherwise.
int isUpper(char theChar)
{
	return theChar >= 65 && theChar <= 90;
}

//Returns 1 iff [theChar] is a lowercase letter, 0 otherwise.
int isLower(char theChar)
{
	return theChar >= 97 && theChar <= 122;
}

//Returns 1 iff [theChar] is an alphabetic letter, 0 otherwise.
int isAlpha(char theChar)
{
	return isLower(theChar) || isUpper(theChar);
}

//Returns 1 iff [theChar] is a digit 0-9, 0 otherwise.
int isDigit(char theChar)
{
	return theChar >= 48 && theChar <= 57;
}

//Returns 1 iff [theChar] is either a digit or an alphabetic letter, 0 otherwise.
int isAlphanumeric(char theChar)
{
	return isAlpha(theChar) || isDigit(theChar);
}

//Returns 1 iff [theChar] is a valid whitespace or invisible character, 0 otherwise.
int isInvisible(char theChar)
{
	return theChar == 9 || theChar == 10 || theChar == 13 || theChar == 32;
}

//Returns 1 iff [theChar] is a valid symbol, 0 otherwise.
int isSymbol(char theChar)
{
	for(int i = 0; i < 13; i++)
	{
		if (symbols[i] == theChar)
			return 1;
	}
	return 0;
}

//Returns 1 iff [theChar] is a valid character used for scanning, 0 otherwise.
int isValid(char theChar)
{
	return isAlphanumeric(theChar) || isSymbol(theChar) || isInvisible(theChar) || theChar == '\0';
}

//If called, this makes a text file called "ef" and places the error message into it.
void throwError(char * message)
{
	FILE * errorFile = fopen("ef", "w");
	printf("An error occurred while running lexical analysis: %s\n", message);
	fprintf(errorFile, "An error occurred while running lexical analysis: %s\n", message);
	fclose(errorFile);
	exit(0);
}

//Gets a character from the input; enforces that the character is valid iff ignoreValidity is 0.
char getChar(int ignoreValidity)
{	
	//Make sure this char is even actually existing...
	if (ip > inputCharsSize)
	{
		throwError("Input file ends unexpectedly! (Did you forget to close a comment?)");
	}

	char nextChar = inputChars[ip];
	ip++;
	
	if (!ignoreValidity && !isValid(nextChar))
	{
		throwError("Invalid character encountered!");
	}
	return nextChar;
}

//Go back!!!
void ungetChar()
{
	ip--;
}

//Empty out that buffer!
void clearBuffer()
{
	bp = 0;
	for(int i = 0; i < 16; i++)
	{
		buffer[i] = '\0';
	}
}

//Add [theChar] to the end of the buffer!
void addToBuffer(char theChar)
{
	if (bp < 16)
	{
		buffer[bp] = theChar;
		bp++;
	}
}

//This method opens the input and output files, and also reads in all the data from the input file.
void openFilesLex(char * inputFile)
{
	FILE * inFile = fopen(inputFile, "r");
    long int inputSize = 0;

	fseek(inFile, 0, SEEK_END);
    inputSize = ftell(inFile);
	inputCharsSize = inputSize;
	fseek(inFile, 0, SEEK_SET);
	int i;
	for(i = 0; i < inputSize; i++)
	{
		fscanf(inFile, "%c", &inputChars[i]);
	}
	inputChars[i] = '\0';
}

//Overwrite all data in the lexeme output arrays!
void clearLexemeOutput()
{
	for(int i = 0; i < MAX_CODE_LENGTH; i++)
	{
		lexemeTable[i] = '\0';
		lexemeList[i] = '\0';
		symbolicLexemeList[i] = '\0';
	}
	strcat(lexemeTable, "Lexeme Table:\nlexeme       token type\n");
	// strcat(lexemeList, "Lexeme List:\n");
	strcat(symbolicLexemeList, "Symbolic Lexeme List:\n");
}

//Insert the lexeme [lexeme] of type [tokenType] nicely into the lexeme table.
void insertToLexemeTable(char * lexeme, int tokenType)
{
	char temp[64];
	char spaces[64] = {'\0'};
	while(strlen(lexeme) + strlen(spaces) < 13)
		strcat(spaces, " ");
	sprintf(temp, "%s%s%d\n", lexeme, spaces, tokenType);
	strcat(lexemeTable, temp);
}

//Insert a number into the lexeme list!
void insertIntToLexemeList(int num)
{
	char temp[64];
	sprintf(temp, "%d ", num);
	strcat(lexemeList, temp);
	sprintf(temp, "%s ", IRMapping[num]);
	strcat(symbolicLexemeList, temp);
}

//Insert a string into the lexeme list!
void insertStrToLexemeList(char * identifier)
{
	char temp[64];
	sprintf(temp, "%s ", identifier);
	strcat(lexemeList, temp);
	strcat(symbolicLexemeList, temp);
}

//Take [identifier] and see if it's a reserved word or actually just an identifier...
void processIdentifier(char * identifier)
{
	int index = reservedIndex(identifier);
	if (index > -1)
	{
		//It's reserved!
		int mapping = mapReserved(index);
		insertIntToLexemeList(mapping);
		insertToLexemeTable(identifier, mapping);
	}
	else
	{
		//Not reserved!
		insertIntToLexemeList(2);
		insertStrToLexemeList(identifier);
		insertToLexemeTable(identifier, 2);
	}
}

//Process a number literal represented by the string pointed to by [num]
void processNumber(char * num)
{
	insertToLexemeTable(num, 3);
	insertIntToLexemeList(3);
	insertStrToLexemeList(num);
}

//Process a symbol represented by the string pointed to by [sym]
void processSymbol(char * sym)
{
	insertToLexemeTable(sym, mapSymbol(sym));
	insertIntToLexemeList(mapSymbol(sym));
}

//The meat of the program, where the actual fancy important scanning stuff happens!
void processText()
{
	FILE *token = fopen("lexTokens.txt", "w");

	//Clear out the output arrays...
	clearLexemeOutput();

	//Run through the input characters...
	char nextChar = ' ';
	while(nextChar != '\0')
	{
		clearBuffer();
		while(isInvisible(nextChar = getChar(0)))
		{
			//Trash the invisible characters
		}

		//It's not invisible if we are here!
		if (isAlpha(nextChar))
		{
			addToBuffer(nextChar);
			while(isAlphanumeric(nextChar = getChar(0)))
			{
				addToBuffer(nextChar);
				if (strlen(buffer) > MAX_IDENTIFIER_LENGTH)
				{
					//Invalid identifier length
					throwError("Identifier too long!");
				}
			}
			ungetChar();
			//Process identifier in buffer
			processIdentifier(buffer);
		}
		else if (isDigit(nextChar))
		{
			addToBuffer(nextChar);
			while(isDigit(nextChar = getChar(0)))
			{
				addToBuffer(nextChar);
				if (strlen(buffer) > MAX_NUMBER_LENGTH)
				{
					//Invalid number length
					throwError("Number too long!");
				}
			}
			//Was this number followed by a letter?
			if (isAlpha(nextChar))
			{
				throwError("Identifier does not start with letter!");
			}
			//It was not followed by a letter.. so we are okay!
			ungetChar();
			//Process number in buffer.
			processNumber(buffer);
		}
		else if (isSymbol(nextChar))
		{
			addToBuffer(nextChar);
			if (nextChar == '+' || nextChar == '-' || nextChar == '*' || nextChar == '(' || nextChar == ')' || nextChar == '=' || nextChar == ',' || nextChar == '.' || nextChar == ';')
			{
				//Process just that symbol itself!
				processSymbol(buffer);
			}
			else if (nextChar == '/')
			{
				nextChar = getChar(0);
				if (nextChar == '*')
				{
					state5:
					while((nextChar = getChar(1)) != '*')
					{
						//Dump comment...
					}
					while((nextChar = getChar(1)) == '*')
					{
						//Dump comment...
					}
					if (nextChar == '/')
					{
						//Done with comment!
					}
					else
					{
						//Still in comment from * to another char...
						goto state5;
					}
				}
				else
				{
					//Process as a divide symbol...
					ungetChar();
					processSymbol(buffer);
				}
			}
			else if (nextChar == '<')
			{
				nextChar = getChar(0);
				if (nextChar == '=')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else if (nextChar == '>')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else
				{
					//Process less only
					ungetChar();
					processSymbol(buffer);
				}
			}
			else if (nextChar == '>')
			{
				nextChar = getChar(0);
				if (nextChar == '=')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else
				{
					//Process greater than only
					ungetChar();
					processSymbol(buffer);
				}
			}
			else if (nextChar == ':')
			{
				//We must have an equal following it!
				nextChar = getChar(0);
				if (nextChar == '=')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else
				{
					throwError("Invalid symbol!");
				}
			}

		}
		else if (nextChar == '\0')
		{
			break;
		}
		else
		{
			//Invalid state
			throwError("Invalid state!");
		}
	}

	//Print results to output file...
	
	//Uncomment this to print out the lexeme table as well...
	//fprintf(outFile, "%s\n", lexemeTable);
	
	if(lexActivate)
	{
		printf("%s\n\n", symbolicLexemeList);
		printf("Lexeme List:\n");
		printf("%s", lexemeList);		
	}

	fprintf(token, "%s", lexemeList);

	fclose(token);
}

void lexicalAnalyzer(char *inputFile)
{
	if(lexActivate)
	{
		printf("LEXICAL ANALYZER\n");
		printf("================\n");		
	}

	openFilesLex(inputFile);

	processText();
}





