#include <stdio.h>                                                              
#include <string.h>                                                             
                                                                                
//#define LINE_MAX 128;                                                         
int countLines(FILE* infile);                                                   
                                                                                
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
                                                                                
    if (containsInvalidCommand(commandQueue))                                   
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
