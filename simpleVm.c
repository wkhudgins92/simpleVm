#include <stdio.h>                                                              
#include <string.h>             
#include <regex.h>                                                
                                                                                
//#define LINE_MAX 128;                                                         
int countLines(FILE* infile);                                                   
int containsInvalidCommand(char commandQueue[][128], int numCommands,
	int* errorLine);
                                                                                
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
    if (containsInvalidCommand(commandQueue, numLines, &errorLine))	                                   
		printf("ERROR: Invalid instruction on line %d\n", errorLine);
// Execute each command sequentially                                            
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
