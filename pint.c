#include "monty.h"

/**
 * pint - Prints the first values on the stack
 * @stack: Pointer to the stack
 * @line_number: Line number in the script
 */
void pint(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;
	(void)line_number;

	printf("%d\n", current->n);
}
