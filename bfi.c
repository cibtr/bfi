#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fputs("Usage: ./bin/bfi [brainfuck file]\n", stderr);
		return EXIT_FAILURE;
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "Error: %s: File not found\n", argv[1]);
		return EXIT_FAILURE;
	}

	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	char *source = malloc(file_size + 1);
	fread(source, sizeof(char), file_size, file);

	fclose(file);

	char current_char = 0;
	int64_t source_index = 0;
	uint32_t loop_depth = 0;

	size_t cells_size = 100;
	uint8_t *cells = calloc(cells_size, sizeof(uint8_t));
	uint8_t *ptr = cells;

	while ((current_char = source[source_index++]) != 0) {
		switch (current_char) {
			case '+':
				++(*ptr);
				break;
			case '-':
				--(*ptr);
				break;
			case '<':
				if (ptr - cells == 0) {
					fputs("Error: Data pointer out of range\n", stderr);
					return EXIT_FAILURE;
				}

				--ptr;
				break;
			case '>':
				if (ptr - cells == cells_size - 1) {
					ptr -= (uintptr_t)cells;

					cells = realloc(cells, cells_size * 2);
					memset(cells + cells_size, 0, cells_size - 1);
	 				cells_size *= 2;

					ptr += (uintptr_t) cells;
				}

				++ptr;
				break;
			case '.':
				putchar(*ptr);
				break;
			case ',':
				*ptr = getchar();
				break;
			case '[':
				if (*ptr != 0)
					continue;
				
				while ((current_char = source[source_index++]) != 0) {
					if (current_char == '[') 
						++loop_depth;
					else if (current_char == ']') {
						if (loop_depth == 0) 
							break;
						else
							--loop_depth;
					}
				}
				break;
			case ']':
				if (*ptr == 0)
					continue;
				
				while (1) {
					source_index -= 2;
					if (source_index < 0) {
						fputs("Error: Unbalanced brackets\n", stderr);
						return EXIT_FAILURE;
					}
					current_char = source[source_index++];
					
					if (current_char == ']') 
						++loop_depth;
					else if (current_char == '[') {
						if (loop_depth == 0) 
							break;
						else 
							--loop_depth;
					}
				}
				break;
			default:
				continue;
		}
	}

	putchar('\n');

	free(source);
	free(cells);

	return EXIT_SUCCESS;
}
