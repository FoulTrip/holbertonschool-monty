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

void monty(char *file_path)
{
	FILE *file = fopen(file_path, "r");
	if (!file)
	{
		printf("Error: No se puede abrir el archivo %s\n", file_path);
		exit(EXIT_FAILURE);
	}

	stack_t *stack = NULL; // Pila inicialmente vacía
	char *input = NULL;
	size_t input_len = 0;

	instructions instr[] = {
		{"push", push},
		{"pall", pall},
		{NULL, NULL}
	};

	while (getline(&input, &input_len, file) != -1)
        {
		line_number++;
                input[strcspn(input, "\n")] = '\0';

                char opcode[256];
                char arg[256];

                if (sscanf(input, "%255s %255s", opcode, arg) == 2)
                {
			int found = 0;
                        for (unsigned int j = 0; instr[j].opcode; j++)
			{
				if (strcmp(instr[j].opcode, opcode) == 0)
                                {
					instr[j].f(&stack, line_number, arg); // Ejecutar la operación correspondiente
                                        found = 1;
                                        break;
                                }
		        }
                        if (!found)
			{
				printf("Error en la línea %u: instrucción desconocida '%s'\n", line_number, opcode);
				exit(EXIT_FAILURE);
                        }
		}
	}

	fclose(file);

	while (stack)
	{
		stack_t *temp = stack;
		stack = stack->next;
		free(temp);
	}

	free (input);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("USO: %s archivo\n", argv[0]);
		return (EXIT_FAILURE);
	}

	char *file_path = argv[1];
	monty(file_path);

	return (0);
}
