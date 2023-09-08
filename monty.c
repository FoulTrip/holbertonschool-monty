#include "monty.h"

/**
 * is_integer - Verifica si una cadena es un número entero válido.
 * @str: Cadena de entrada a verificar
 * Return: 1 si es un número entero válido, 0 en caso contrario.
 */
int is_integer(const char *str)
{
	if (*str == '-' || *str == '+')
		str++; /* Permite un signo negativo o positivo al principio */
	if (*str == '\0')
		return (0); /* La cadena está vacía */

	for (; *str != '\0'; str++)
	{
		if (!isdigit(*str))
			return (0); /* No es un dígito */
	}
	return (1); /* Es un número entero */
}

/**
 * free_resources - Libera los recursos al finalizar el programa.
 * @file: Puntero al archivo abierto
 * @line: Puntero a la línea leída
 * @stack: Puntero a la pila
 */
void free_resources(FILE *file, char *line, stack_t **stack)
{
	if (file)
		fclose(file); /* Cierra el archivo si está abierto */
	if (line)
		free(line); /* Libera la memoria asignada a la línea leída */
	if (*stack)
		free_dlistint(*stack); /* Libera la memoria de la pila */
}

/**
 * main - Monty interpreter main function
 * @argc: Argument count
 * @argv: Argument values
 * Return: 0 on success, 1 on failure
 */

int main(int argc, char *argv[])
{
	char *opcode, *value, *line = NULL;
        size_t line_number = 0;
	size_t len = 0;
        FILE *file;

        if (argc != 2)
        {
		fprintf(stderr, "USAGE: monty file\n");
                return (EXIT_FAILURE);
        }

	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		return (EXIT_FAILURE);
	}
	stack_t *stack = NULL;

	while (getline(&line, &len, file) != -1)
        {
		line_number++;
		opcode = strtok(line, " \t$\n");

                if (!opcode || opcode[0] == '#')
			continue;

		value = strtok(NULL, " $\n");
		
		if (!value || (strcmp(opcode, "push") == 0 && !is_integer(value)))
                {
			fprintf(stderr, "L%lu: usage: push integer\n", line_number);
                        free_resources(file, line, &stack);
                        return (EXIT_FAILURE);
		}

		if (strcmp(opcode, "push") == 0)
			push(&stack, atoi(value));
		else if (strcmp(opcode, "pall") == 0)
			pall(&stack, line_number);
		else if (strcmp(opcode, "pint") == 0)
			pint(&stack, line_number);
		else if (strcmp(opcode, "pop") == 0)
			pop(&stack, line_number);
		else if (strcmp(opcode, "swap") == 0)
			swap(&stack, line_number);
		else if (strcmp(opcode, "add") == 0)
			add(&stack, line_number);
		else if (strcmp(opcode, "nop") != 0)
		{
			fprintf(stderr, "L%lu: unknown instruction %s\n", line_number, opcode);
			free_resources(file, line, &stack);
			return (EXIT_FAILURE);
		}
	}

	free_resources(file, line, &stack);
	return (EXIT_SUCCESS);
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
 * free_dlistint - function that frees a list
 * @head: pointer to the header of the nodes
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
 * swap - Swaps the top two elements of the stack
 * @stack: Puntero a la pila
 * @line_number: Número de línea en el script
 */
void swap(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	int tmp = (*stack)->n;
	(*stack)->n = (*stack)->next->n;
	(*stack)->next->n = tmp;
}

/**
 * add - Adds the top two elements of the stack
 * @stack: Puntero a la pila
 * @line_number: Número de línea en el script
 */
void add(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	(*stack)->next->n += (*stack)->n; /* Suma los dos elementos superiores */
	pop(stack, line_number); /* Elimina el elemento superior */
}

/**
 * nop - Doesn't do anything
 * @stack: Puntero a la pila
 * @line_number: Número de línea en el script
 */
void nop(stack_t **stack, unsigned int line_number)
{
    (void)stack;
    (void)line_number;
}
