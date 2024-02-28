#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Structure for storing shell information */
typedef struct info_s {
    char *arg;             /* Argument string */
    char **argv;           /* Argument vector */
    char *path;            /* Executable path */
    int argc;              /* Argument count */
    char *fname;           /* File name */
    int readfd;            /* File descriptor for reading */
    int env_changed;       /* Flag indicating environment changes */
    list_t *env;           /* Environment variables */
    list_t *history;       /* Command history */
    list_t *alias;         /* Command aliases */
    char **environ;        /* Copy of environment variables */
    char **cmd_buf;        /* Command buffer */
    int histcount;         /* History count */
} info_t;

/* Structure for linked list */
typedef struct list_s {
    char *str;             /* String */
    int num;               /* Number */
    struct list_s *next;   /* Pointer to the next node */
} list_t;

/* Function prototypes */
int interactive(info_t *info);
int is_delim(char c, char *delim);
int _isalpha(int c);
int _atoi(char *s);

#endif /* SHELL_H */

