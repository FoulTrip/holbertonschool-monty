#include "monty.h"

/**
 * free_dlistint - function that frees a list
 * @head: Pointer to the header of the nodes
 * Return: void
 */
void free_dlistint(stack_t *stack)
{
	stack_t *tmp;
	while (stack != NULL)
	{
		tmp = stack;
		stack = stack->next;
		free(tmp);
	}
}
