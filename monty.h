#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack_t
{
	int n;
	struct stack_t *prev;
	struct stack_t *next;
} stack_t;

typedef struct instructions
{
	char *opcode;
	void (*f)(stack_t **stack, unsigned int line_number, char *arg);
} instructions;

void push(stack_t **stack, unsigned int line_number, char *arg);
void pall(stack_t **stack, unsigned int line_number, char *arg);

#endif
