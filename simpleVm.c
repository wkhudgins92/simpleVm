/*
 * Implements a simple stack based VM and provides a 7 instruction ISA for the
 * VM.
 */

#include <stdlib.h>
#include <stdio.h>                                                              
#include <string.h>             
#include <regex.h>                                                
                                                                                
#define LINE_MAX 128                                                         
#define STACK_SIZE 100
#define INSTRUCTION_SET_SIZE 7

int countLines(FILE* infile);                                                   
int containsInvalidCommand(char commandQueue[][LINE_MAX], int numCommands,
	int* errorLine);
void executeCode(char commandQueue[][LINE_MAX], int numCommands);

/**
 * Main function for this program. 
 * Expects an accessible file to be passed in argv for execution.
 */
int main (int argc, char* argv[]) {                                             
    int i, numLines;                                                               
    char* scriptName;                                                           
    FILE* sourceCode;                                                           
                                                                                
    // Insufficient number of arguments supplied                                                                            
    if (argc != 2) {                                                            
        printf("Usage: simpleVm script.simple\n");                              
        return 1;                                                               
    }                                                                           
    
	// Get the name of the script                                                                            
    scriptName = argv[1];        

	// If file cannot be opened                                               
    if (!(sourceCode = fopen(scriptName, "r"))) {                               
        printf("Error accessing %s\n", scriptName);                             
        return 1;                                                               
    }                                                                           
   
	// Read each command into an array 
    numLines = countLines(sourceCode);                                          
    if (numLines == 0)                                                          
        return 0;                                                               
                                                                                
    rewind(sourceCode);                                                         
    char commandQueue[numLines][LINE_MAX];                                       
    i = 0;                                                                      
    while (fgets(commandQueue[i], LINE_MAX, sourceCode)) {                           
        commandQueue[i][strlen(commandQueue[i]) - 1] = '\0';                    
        i++;                                                                    
    }                                                                           
               
	// Ensure that each line is synatically correct                                                                            
	int errorLine;  
    if (containsInvalidCommand(commandQueue, numLines, &errorLine)) {	                                   
		printf("ERROR: Invalid instruction on line %d\n", errorLine);
		return 1;
	}

	// Execute code
	executeCode(commandQueue, numLines);

	return 0;
}            

/**
 * Executes a SIMPLE script.
 * INPUT:
 * char commandQueue[][LINE_MAX]: An array containing all commands to be executed
 * int numCommands: Number of commands in queue
 */
void executeCode(char commandQueue[][LINE_MAX], int numCommands) {
	int i, j, reti, temp;
	int stackSize = 0;
	int memoryStack[STACK_SIZE]; // Stack only can go 100 deep
	regex_t regex[INSTRUCTION_SET_SIZE];
	char* validCommandRegex[] = {"PUSH [1-9][0-9]*", "POP", "ADD", 
		"IFEQ [1-9][0-9]*", "JUMP [1-9][0-9]*", "PRINT", "DUP"};
	char* tempString = malloc(sizeof(char) * LINE_MAX); 

	// Compile regular expressions
	for (i = 0; i < INSTRUCTION_SET_SIZE; i++)
		reti = regcomp(&regex[i], validCommandRegex[i], 0);

	// Loop through each instruction in the command queue
	for (i = 0; i < numCommands; i++) 
		for (j = 0; j < INSTRUCTION_SET_SIZE; j++) { 
			reti = regexec(&regex[j], commandQueue[i], 0, NULL, 0);
			if (!reti) {
				switch (j) {
					case 0: // PUSH: Adds item to top of stack
						if (stackSize >= STACK_SIZE - 1) {
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
					case 1: // POP: Removes item from top of stack
						if (stackSize < 1) {
							printf("Fatal error: Cannot pop from empty stack.\n");
							exit(1);
						}

						stackSize--;
						break;
					case 2: // ADD: Adds top two items on the stack together, 
							// removes thsoe values, and places new value on 
							// top of stack
						if (stackSize < 2) {
							printf("Fatal error: Missing one operand.\n");
							exit(1);
						}

						temp = memoryStack[stackSize - 1] + memoryStack[stackSize - 2];
						stackSize--;
						memoryStack[stackSize - 1] = temp;
						break;
					case 3: // IFEQ: If top of stack is 0, jumps to another instruction
						if (memoryStack[stackSize - 1] == 0) {
							tempString = strcpy(tempString, commandQueue[i]);
							strtok(tempString, " ");
							tempString = strtok(NULL, " ");
							temp = atoi(tempString);
							i = temp - 2; // -2 because i is incremented at end of loop
						}		
						break;
					case 4: // JUMP: Jumps to a specific instruction
						tempString = strcpy(tempString, commandQueue[i]);
						strtok(tempString, " ");
						tempString = strtok(NULL, " ");
						temp = atoi(tempString);
						i = temp - 2;
						break; 
					case 5: // PRINT: Outputs value of top item on stack
						printf("%d\n", memoryStack[stackSize - 1]);
						break;
					case 6: // DUP: Duplicates top item of the stack
						stackSize++;
						memoryStack[stackSize - 1] = memoryStack[stackSize - 2];
						break;
			}
		}	
	}

	return;
}                                                                   

/**
 * Counts the number of lines in a file.
 * INPUT:
 * FILE* infile: The file to count lines in
 * RETURN VALUE:
 * The number of lines in the file
 */ 
int countLines(FILE* infile) {                                                  
    int currentChar;                                                            
    int previousChar;                                                           
    int lineCount = 0;                                                          
                                                                                
    while ((currentChar = fgetc(infile)) != EOF)                                
        if (currentChar == '\n')                                                
            lineCount++;                                                        
    return lineCount;                                                           
}              

/**
 * Takes a list of commands and ensures they are valid.
 * INPUT:
 * char CommandQueue[][LINE_MAX]: A list of commands
 * int numCommands: How many commands there are
 * int* errorLine: A pointer to store the line number in which an error is found
 * RETURN VALUE:
 * Returns 0 if there are no invalid commands and 1 if there is any invalid command.
 * Also updates the errorLine pointer, which reflects the first line where an error was found.
 */
int containsInvalidCommand(char commandQueue[][LINE_MAX], int numCommands, 
	int* errorLine) {
	int i, flag, reti;
	char* validCommandRegex[] = {"PUSH [1-9][0-9]*", "POP", "ADD", 
		"IFEQ [1-9][0-9]*", "JUMP [1-9][0-9]*", "PRINT", "DUP"};
	regex_t regex[INSTRUCTION_SET_SIZE];

	// Compile regular expressions
	for (i = 0; i < INSTRUCTION_SET_SIZE; i++)
		reti = regcomp(&regex[i], validCommandRegex[i], 0);

	// Check each command by each regular expression to ensure all commands valid
	for (i = 0; i < numCommands; i++) {
		flag = 0;
		for (int j = 0; !flag && j < INSTRUCTION_SET_SIZE; j++) {
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
