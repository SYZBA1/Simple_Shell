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


/* Function Prototypes */
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *starts_with(const char *haystack, const char *needle);
char *_strcat(char *dest, char *src);

char *_memset(char *s, char b, unsigned int n);
void ffree(char **pp);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int hsh(ino_t *ifno, char **av);
int find_builtin(ino_t *info);
void find_cmd(ino_t *info);
void fork_cmd(ino_t *info);

char *_strcpy(char *dest, char *src);
char *_strdup(const char *str);
void _puts(char *str);
int _putchar(char c);

#endif /* SHELL_H */

