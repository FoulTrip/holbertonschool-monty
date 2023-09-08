#include "monty.h"

/**
 * pop - removes the first values on the stack
 * @stack: Pointer to the stack
 * @line_number: Line number in the script
 */
void pop(stack_t **stack, unsigned int line_number)
{
	stack_t *node_to_delete = *stack;
	(void)line_number;

	*stack = node_to_delete->next;
	if (node_to_delete->next)
		node_to_delete->next->prev = NULL;
	free(node_to_delete);
}
