#include "monty.h"
/**
 * main - Monty interpreter main function
 * @argc: Argument count
 * @argv: Argument values
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
	char *opcode, *value, *line = NULL;
	size_t line_number = 0, len = 0, num_of_nodes, nodes;
	FILE *file;
	int i, sum;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	while (getline(&line, &len, file) != -1)
	{
		line_number++;
		opcode = strtok(line, " \t$\n");
		if (opcode == NULL || opcode[0] == '#')
			continue;
		if (strcmp(opcode, "push") == 0)
		{
			value = strtok(NULL, " $\n");
			if (value == NULL)
			{
				fprintf(stderr, "L%lu: usage: push integer\n", line_number);
				fclose(file);
				free(line);
				exit(EXIT_FAILURE);
			}
			for (i = 0; value[i] != '\0'; i++)
			{
				if (value[i] != '-' && !isdigit(value[i]))
				{
					fprintf(stderr, "L%lu: usage: push integer\n", line_number);
					fclose(file);
					free(line);
					exit(EXIT_FAILURE);
				}
			}
			push(&stack, atoi(value));
		}
		else if (strcmp(opcode, "pall") == 0)
		{
			pall(&stack, line_number);
		}
		else if (strcmp(opcode, "pint") == 0)
		{
			if (stack == NULL)
			{
				fprintf(stderr, "L%lu: can't pint, stack empty\n", line_number);
				fclose(file);
				exit(EXIT_FAILURE);
			}
			pint(&stack, line_number);
		}
		else if (strcmp(opcode, "pop") == 0)
		{
			if (stack == NULL)
			{
				fprintf(stderr, "L%lu: can't pop an empty stack\n", line_number);
				fclose(file);
				exit(EXIT_FAILURE);
			}

			 pop(&stack, line_number);
		}
		else if (strcmp(opcode, "swap") == 0)
		{
			num_of_nodes = dlistint_len(stack);
			if (num_of_nodes < 2)
			{
				fprintf(stderr, "L%lu: can't swap, stack too short\n", line_number);
				exit(EXIT_FAILURE);
			}
			swap(&stack, line_number);
		}
		else if (strcmp(opcode, "add") == 0)
		{
			nodes = dlistint_len(stack);
			if (nodes < 2)
			{
				fprintf(stderr, "L%lu: can't add, stack too short\n", line_number);
				exit(EXIT_FAILURE);
			}
			sum = sum_dlistint(stack);
			delete_dnodeint_at_index(&stack, 0);
			stack->n = sum;
		}
		else if (strcmp(opcode, "nop") == 0)
		{
			nop(&stack, line_number);
		}
		else
		{
			fprintf(stderr, "L%lu: unknown instruction %s\n", line_number, opcode);
			fclose(file);
			free(line);
			free_dlistint(stack);
			exit(EXIT_FAILURE);
		}
		free(line);
		line = NULL;
	}
	free_dlistint(stack);
	free(line);
	fclose(file);
	return (0);
}
/**
 * push - Pushes an element onto the stack
 * @stack: Pointer to the stack
 * @value: Value to be pushed onto the stack
 */
void push(stack_t **stack, int value)
{
	stack_t *new_node;
	new_node = malloc(sizeof(stack_t));
	if (new_node == NULL)
	{
		fprintf(stderr, "Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	new_node->n = value;
	new_node->prev = NULL;
	new_node->next = *stack;
	if (*stack != NULL)
		(*stack)->prev = new_node;
	*stack = new_node;
}
/**
 * pall - Prints all values on the stack
 * @stack: Pointer to the stack
 * @line_number: Line number in the script
 */
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;
	(void)line_number; /*Parametro no utilizado*/
	while (current != NULL)
	{
		printf("%d\n", current->n);
		current = current->next;
	}
}

/**
 * pint - Prints the first values on the stack
 * @stack: Pointer to the stack
 * @line_number: Line number in the script
 */
void pint(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;
	(void)line_number; /*Parametro no utilizado*/

	printf("%d\n", current->n);
}

/**
 * pop - removes the first values on the stack
 * @stack: Pointer to the stack
 * @line_number: Line number in the script
 */
void pop(stack_t **stack, unsigned int line_number)
{
	stack_t *node_to_delete = *stack;
	(void)line_number;/*Parametro no Utilizado*/

	*stack = node_to_delete->next;
	if (node_to_delete->next)
		node_to_delete->next->prev = NULL;
	free(node_to_delete);
}

/**
 * swap - Swaps for the nodes
 * @stack: Pointer to the stack
 * @line_number: Line number in the script
 */
void swap(stack_t **stack, unsigned int line_number)
{
	stack_t *node_to_swap = *stack;
	(void)line_number;/*Parametro no Utilizado*/
	int num_of_nodes = dlistint_len(*stack);

	if (num_of_nodes >=3)
	{
		*stack = node_to_swap->next;
		(*stack)->prev = NULL;
		node_to_swap->prev = (*stack)->next->prev;
		(*stack)->next->prev = node_to_swap;
		node_to_swap->next = (*stack)->next;
		(*stack)->next = node_to_swap;
	}
	else
	{
		*stack = node_to_swap->next;
		(*stack)->prev = NULL;
		node_to_swap->prev = node_to_swap->next;
		(*stack)->next = node_to_swap;
		node_to_swap->next = NULL;
	}

}

/**
 * free_dlistint - function that frees a list
 * @stack: pointer to the header of the nodes
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
/**
 * dlistint_len - function that returns the number of elements in a linked list
 * @h: pointer to the header of the nodes
 * Return: the numbers of nodes
 */
size_t dlistint_len(const stack_t *stack)
{
	size_t i = 0;
	const stack_t *actual = stack;

	while (actual != NULL)
	{
		actual = actual->next;
		i++;
	}
	return (i);
}
/**
 * sum_dlistint - function that adds the top two elements of the stack
 * @stack : pointer to list
 * Return: sum of top two elements or the stack
 */
int sum_dlistint(stack_t *stack)

{
	int sum = 0, i = 0;

	stack_t *current = stack;

	while (i < 2)
	{
		sum += current->n;
		current = current->next;
		i++;
	}
	return (sum);
}

/**
 * delete_dnodeint_at_index - func that delet a new node at a given position
 * @head: pointer to the header of the nodes
 * @index: the index of the list where the new node should be added
 * Return: 1 if it succeeded, -1 if it failed
 */
int delete_dnodeint_at_index(stack_t **stack, unsigned int index)
{
	unsigned int i;
	stack_t *node_to_delete;

	if (!stack || !*stack)
		return (-1);
	node_to_delete = *stack;

	if (index == 0)
	{
		*stack = node_to_delete->next;
		if (node_to_delete->next)
			node_to_delete->next->prev = NULL;
		free(node_to_delete);
		return (1);
	}
	for (i = 0; node_to_delete && i < index; i++)
	{
		node_to_delete = node_to_delete->next;
	}
	if (!node_to_delete)
		return (-1);
	if (!node_to_delete->next)
	{
		node_to_delete->prev->next = NULL;
		free(node_to_delete);
		return (1);
	}
	node_to_delete->prev->next = node_to_delete->next;
	node_to_delete->next->prev = node_to_delete->prev;
	free(node_to_delete);
	return (1);
}
/**
 *nop- Function that doesn´t do anything
 *@stack: stack struct
 *@line_number: line_number
 *Return: no return
 */
void nop(stack_t **stack, unsigned int line_number)
{
	(void)stack;
	(void)line_number;
}
