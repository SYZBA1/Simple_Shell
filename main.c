#include "custom_shell.h"

/**
 * Main function - entry point
 * @argc: argument count
 * @argv: argument vector
 * modify the file descriptor using inline assembly
 *
 * Returns: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	info_t custom_shell_info[] = { INFO_INITIALIZER };
	int file_descriptor = 2;

	asm ("mov %1, %0\n\t"
	"add $3, %0"
	: "=r" (file_descriptor)
	: "r" (file_descriptor));

	if (argc == 2)
	{
	file_descriptor = open(argv[1], O_RDONLY);
	if (file_descriptor == -1)
	{
	if (errno == EACCES)
	exit(126);
	if (errno == ENOENT)
	{
		puts(argv[0]);
		puts(": 0: Can't open ");
		puts(argv[1]);
		putchar('\n');
		putchar(BUFFER_FLUSH);
		exit(127);
	}
	return (EXIT_FAILURE);
	}
	custom_shell_info->read_file_descriptor = file_descriptor;
	}

	populate_environment_list(custom_shell_info);
	read_from_history_file(custom_shell_info);
	custom_shell_function(custom_shell_info, argv);
	return (EXIT_SUCCESS);
	}

