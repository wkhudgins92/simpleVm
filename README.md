# simpleVm
Contains a simple stack-based VM
## Instruction SET
### PUSH `<integer>`
Pushes the supplied integer to the top of the stack.
### POP
Pops the top item off of the stack.
### SUM
Calculates the sum of the first two items on the stack, removes those items, and pushes the sum on to the stack. 
### IFEQ `<integer>`
If the top of the stack is 0, skip to the supplied instruction number.
### JUMP `<integer>`
Skip to the supplied instruction number.
### PRINT
Print the top value of the stack to the screen.
### DUP
Push a copy of the top stack value onto the stack.
