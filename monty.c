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
