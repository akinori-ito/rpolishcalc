#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 1024
#define MAX_TOKEN 128
#define MAX_STACK 1024

int Stack[MAX_STACK];
int SP = 0;

int split_token(char *buffer,char **tokens)
{
    int n = 0;
    int i = 0;
    while (n < MAX_TOKEN) {
	while (i < BUFFERSIZE && isspace(buffer[i])) i++;
	if (buffer[i] == '\0' || i == BUFFERSIZE) return n;
	tokens[n++] = &buffer[i];
	while (i < BUFFERSIZE-1 && !isspace(buffer[i])) i++;
	buffer[i] = '\0';
	i++;
    }
    return n;
}

int numeric(char *x, int *n)
{
    int i = 0;
    *n = 0;
    int sign = 1;
    if (x[i] == '\0') return 0;
    if (x[i] == '-') {
	sign = -1;
	i++;
    }
    while (isdigit(x[i])) {
	*n = *n*10+x[i++]-'0';
    }
    if (x[i] == '\0') {
	*n *= sign;
	return 1;
    }
    return 0;
}

int push(int n)
{
    if (SP == MAX_STACK) {
        fprintf(stderr,"Stack overflow\n");
	return 0;
    }
    Stack[SP++] = n;
    return 1;
}

int pop(int *n)
{
    if (SP == 0) {
	fprintf(stderr,"Stack underflow\n");
	return 0;
    }
    *n = Stack[--SP];
    return 1;
}

void interpret(char **tokens, int ntoken)
{
    int n;
    for (int i = 0; i < ntoken; i++) {
	int x,y;
	/* printf("%d: %s\n",i,tokens[i]); */
	if (numeric(tokens[i],&n)) {
	    if (!push(n)) return;
	} else if (strcmp(tokens[i],"+") == 0) {
	    if (!pop(&x)) return;
	    if (!pop(&y)) return;
	    if (!push(x+y)) return;
	} else if (strcmp(tokens[i],"-") == 0) {
	    if (!pop(&x)) return;
	    if (!pop(&y)) return;
	    if (!push(x-y)) return;
	} else if (strcmp(tokens[i],"*") == 0) {
	    if (!pop(&x)) return;
	    if (!pop(&y)) return;
	    if (!push(x*y)) return;
	} else if (strcmp(tokens[i],"/") == 0) {
	    if (!pop(&x)) return;
	    if (!pop(&y)) return;
	    if (!push(x/y)) return;
	} else if (strcmp(tokens[i],".") == 0) {
	    if (!pop(&x)) return;
	    printf("%d\n",x);
	} else if (strcmp(tokens[i],"dup") == 0) {
	    if (!pop(&x)) return; 
	    if (!push(x)) return; 
	    if (!push(x)) return; 
	} else if (strcmp(tokens[i],"pop") == 0) {
	    if (!pop(&x)) return;
	} else if (strcmp(tokens[i],"q") == 0) {
            exit(0);
	} else {
	    fprintf(stderr,"Unknown token %s\n",tokens[i]);
	    return;
	}
	/* printf("Stack: "); for (int k = 0; k < SP; k++) printf("%d ",Stack[k]); printf("\n");*/
    }
}


void main()
{
    static char buffer[BUFFERSIZE];
    static char *tokens[128];

    while (1) {
	    if (fgets(buffer,BUFFERSIZE,stdin) == NULL) break;
	    int ntoken = split_token(buffer,tokens);
	    interpret(tokens,ntoken);
    }
}
