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

void push(stack_t **stack, unsigned int line_number, char *arg)
{
	int value = 0;
	stack_t *new_node = malloc(sizeof(stack_t));

	if (!new_node)
	{
		printf("Error al asignar memoria\n");
		exit(EXIT_FAILURE);
	}

	if (!arg || (value = atoi(arg)) == 0)
	{
		printf("Error en la linea %u: Solo numeros enteros\n", line_number);
		free(new_node);
		exit(EXIT_FAILURE);
	}

	new_node->n = value;
	new_node->prev = NULL;
	new_node->next = *stack;

	if (*stack)
	{
		(*stack)->prev = new_node;
	}

	*stack = new_node;
}

void pall(stack_t **stack, unsigned int line_number, char *arg)
{
	(void)line_number;
	(void)arg;
	stack_t *current = *stack;

	while (current)
	{
		printf("%d\n", current->n);
		current = current->next;
	}
}

int main()
{
	stack_t **stack = NULL;
	unsigned int line_number = 0;

	instructions instr[] = {
		{"push", push},
		{"pall", pall},
		{NULL, NULL}
	};

	char *input = NULL;
	size_t input_len = 0;

	while (1)
	{
		line_number++;
		printf("ingrese a una instruccion: ");

		ssize_t read = getline(&input, &input_len, stdin);

		if (read == -1)
		{
			break;
		}

		input[strcspn(input, "\n")] = '\0';

		char opcode[256];
		char arg[256];

		if (sscanf(input, "%255s %255s", opcode, arg) == 2)
		{
			for (unsigned int j = 0; instr[j].opcode; j++)
			{
				if (strcmp(opcode, instr[j].opcode) == 0)
				{
					instr[j].f(&stack, line_number, arg);
					break;
				}
				else if (instr[j].opcode == NULL)
				{
					printf("Error en la linea %u: Instruccion desconocida '%s'\n", line_number, opcode);
				}
			}
		}
		else if (strcmp(input, "pall") == 0)
		{
			pall(&stack, line_number, NULL);
		}
	}

	while (stack)
	{
		stack_t *temp = stack;
		stack = stack->next;
		free(temp);
	}

	free(input);
	return (0);
}
