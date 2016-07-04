#include <stdlib.h>
#include <stdio.h>                                                              
#include <string.h>             
#include <regex.h>                                                
                                                                                
//#define LINE_MAX 128;                                                         
int countLines(FILE* infile);                                                   
int containsInvalidCommand(char commandQueue[][128], int numCommands,
	int* errorLine);
void executeCode(char commandQueue[][128], int numCommands);
                                                                                
int main (int argc, char* argv[]) {                                             
    FILE* sourceCode;                                                           
    char* scriptName;                                                           
    int numLines;                                                               
    int i;                                                                      
                                                                                
                                                                                
    if (argc != 2) {                                                            
        printf("Usage: simpleVm script.simple\n");                              
        return 1;                                                               
    }                                                                           
                                                                                
    scriptName = argv[1];                                                       
    if (!(sourceCode = fopen(scriptName, "r"))) {                               
        printf("Error accessing %s\n", scriptName);                             
        return 1;                                                               
    }                                                                           
                                                                                
    numLines = countLines(sourceCode);                                          
    if (numLines == 0)                                                          
        return 0;                                                               
                                                                                
    rewind(sourceCode);                                                         
    char commandQueue[numLines][128];                                           
    i = 0;                                                                      
    while (fgets(commandQueue[i], 128, sourceCode)) {                           
        commandQueue[i][strlen(commandQueue[i]) - 1] = '\0';                    
        i++;                                                                    
    }                                                                           
               
	int errorLine;  
    if (containsInvalidCommand(commandQueue, numLines, &errorLine)) {	                                   
		printf("ERROR: Invalid instruction on line %d\n", errorLine);
		return 1;
	}

	// Execute code
	executeCode(commandQueue, numLines);

	return 0;
}            

void executeCode(char commandQueue[][128], int numCommands) {
	int memoryStack[100]; // Stack only can go 100 deep
	int stackSize = 0;
	int i;
	int j;
	int reti;
	int temp;
	char* tempString = malloc(sizeof(char) * 128); 
	regex_t regex[7];
	char* validCommandRegex[] = {"PUSH [1-9][0-9]*", "POP", "ADD", 
		"IFEQ [1-9][0-9]*", "JUMP [1-9][0-9]*", "PRINT", "DUP"};

	for (i = 0; i < 7; i++)
		reti = regcomp(&regex[i], validCommandRegex[i], 0);

	for (i = 0; i < numCommands; i++) 
		for (j = 0; j < 7; j++) {
			reti = regexec(&regex[j], commandQueue[i], 0, NULL, 0);
			if (!reti) {
				switch (j) {
					case 0: // PUSH
						if (stackSize >= 99) {
							printf("Fatal error: Can't push to full stack.\n");
							exit(1);
						}

						tempString = strcpy(tempString, commandQueue[i]);
						strtok(tempString, " ");
						tempString = strtok(NULL, " ");

						temp = atoi(tempString);
						memoryStack[stackSize] = temp;
						stackSize++;
						break;
					case 1: // POP
						if (stackSize < 1) {
							printf("Fatal error: Cannot pop from empty stack.\n");
							exit(1);
						}

						stackSize--;
						break;
					case 2: // ADD
						if (stackSize < 2) {
							printf("Fatal error: Missing one operand.\n");
							exit(1);
						}

						temp = memoryStack[stackSize - 1] + memoryStack[stackSize - 2];
						stackSize--;
						memoryStack[stackSize - 1] = temp;
						break;
					case 3: // IFEQ
						if (memoryStack[stackSize - 1] == 0) {
							tempString = strcpy(tempString, commandQueue[i]);
							strtok(tempString, " ");
							tempString = strtok(NULL, " ");
							temp = atoi(tempString);
							i = temp - 2;
						}		
						break;
					case 4: // JUMP
						tempString = strcpy(tempString, commandQueue[i]);
						strtok(tempString, " ");
						tempString = strtok(NULL, " ");
						temp = atoi(tempString);
						i = temp - 2;
						break; 
					case 5: // PRINT
						printf("%d\n", memoryStack[stackSize - 1]);
						break;
					case 6: // DUP
						stackSize++;
						memoryStack[stackSize - 1] = memoryStack[stackSize - 2];
						break;
			}
		}	
	}

	return;
}                                                                   
                                                                                
int countLines(FILE* infile) {                                                  
    int currentChar;                                                            
    int previousChar;                                                           
    int lineCount = 0;                                                          
                                                                                
    while ((currentChar = fgetc(infile)) != EOF)                                
        if (currentChar == '\n')                                                
            lineCount++;                                                        
    return lineCount;                                                           
}              

int containsInvalidCommand(char commandQueue[][128], int numCommands, 
	int* errorLine) {
	int reti;
	int flag;
	int i;
	regex_t regex[7];
	char* validCommandRegex[] = {"PUSH [1-9][0-9]*", "POP", "ADD", 
		"IFEQ [1-9][0-9]*", "JUMP [1-9][0-9]*", "PRINT", "DUP"};

	for (i = 0; i < 7; i++)
		reti = regcomp(&regex[i], validCommandRegex[i], 0);

	for (i = 0; i < numCommands; i++) {
		flag = 0;
		for (int j = 0; !flag && j < 7; j++) {
			reti = regexec(&regex[j], commandQueue[i], 0, NULL, 0);
			if (!reti) 
				flag = 1;
		}

		if (!flag) {
			*errorLine = i + 1;
			return 1;
		}
	}

	return 0;
} 
