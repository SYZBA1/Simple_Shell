#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/* Macros */
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1
#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096
#define CONVERT_UNSIGNED 1
#define CONVERT_LOWERCASE 2

/* Structs */
typedef struct info
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	char **environ;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **cmd_buf;
	char *fname;
	int line_count;
	int readfd;
	int histcount;
	int env_changed;
	int status; // Added for storing the status of the last executed command
	int linecount_flag; // Added to track if a line count was incremented
	int err_num; // Added to store error number
} info_t;

typedef struct list
{
	char *str;
	int num;
	struct list *next;
} list_t;

/* Function Prototypes */
// Previous function prototypes...
char *_memset(char *s, char b, unsigned int n);
void ffree(char **pp);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int hsh(info_t *info, char **av);
int find_builtin(info_t *info);
void find_cmd(info_t *info);
void fork_cmd(info_t *info);

#endif /* SHELL_H */

