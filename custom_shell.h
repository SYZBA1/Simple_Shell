#ifndef _CUSTOM_SHELL_H_
#define _CUSTOM_SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Buffer sizes for reading and writing */
#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024
#define BUFFER_FLUSH -1

/* Command chaining types */
#define COMMAND_NORMAL 0
#define COMMAND_OR 1
#define COMMAND_AND 2
#define COMMAND_CHAIN 3

/* Conversion flags for convert_number() */
#define CONVERT_TO_LOWERCASE 1
#define CONVERT_TO_UNSIGNED 2

/* Flags for system functions usage */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE ".custom_shell_history"
#define MAX_HISTORY_ENTRIES 4096

extern char **custom_environment;

/**
 * Struct for singly linked list node containing a number and a string
 */
typedef struct string_node
{
    int number;
    char *string;
    struct string_node *next;
} string_list;

/**
 * Struct to hold various information to be passed into functions
 */
typedef struct function_info
{
    char *arguments;
    char **argument_vector;
    char *executable_path;
    int argument_count;
    unsigned int line_counter;
    int error_number;
    int line_count_flag;
    char *file_name;
    string_list *environment;
    string_list *command_history;
    string_list *unset_custom_alias_list;
    char **custom_environment;
    int environment_changed;
    int execution_status;

    char **command_buffer;
    int command_buffer_type;
    int read_file_descriptor;
    int history_count;
} info_t;

#define INFO_INITIALIZER \
{ \
    NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0 \
}

/**
 * Struct for built-in commands containing the command string and the associated function
 */
typedef struct built_in_command
{
    char *command;
    int (*function)(info_t *);
} built_in_command_table;

/* Function declarations */

int custom_shell_function(info_t *, char **);
int find_builtin_command(info_t *);
void find_executable(info_t *);
void execute_command(info_t *);

void print_error_message(char *);
int print_character(int);
int write_to_file_descriptor(char, int);
int write_string_to_file_descriptor(char *, int);

int string_length(char *);
int string_compare(char *, char *);
char *string_starts_with(const char *, const char *);
char *string_concatenate(char *, char *);

char *string_copy(char *, char *);
char *string_duplicate(const char *);
void print_string(char *);
int print_character(int);

char *string_copy_n(char *, char *, int);
char *string_concatenate_n(char *, char *, int);
char *string_find_character(char *, char);

char **tokenize_string(char *, char *);
char **tokenize_string_v2(char *, char);

void *reallocate_memory(void *, unsigned int, unsigned int);
char *memory_set(char *, char, unsigned int);
void free_memory(char **);
int free_block(void **);

int convert_string_to_integer(char *);
int is_delimiter(char, char *);
int is_alphabetic(int);
int convert_string_to_integer(char *);

int interactive_mode(info_t *);
void clear_function_info(info_t *);
void set_function_info(info_t *, char **);
void free_function_info(info_t *, int);

char *_get_environment_variable(info_t *, const char *);
int show_environment(info_t *);
int set_environment_variable(info_t *);
int unset_environment_variable(info_t *);
int populate_environment_list(info_t *);

char **get_custom_environment(info_t *);
int unset_custom_environment_variable(info_t *, char *);
int set_custom_environment_variable(info_t *, char *, char *);

char *get_history_file_path(info_t *);
int write_to_history_file(info_t *);
int read_from_history_file(info_t *);
int build_history_list(info_t *, char *, int);
int renumber_history_entries(info_t *);

string_list *add_node_to_list(string_list **, const char *, int);
string_list *add_node_to_list_end(string_list **, const char *, int);
size_t print_string_list(const string_list *);
int delete_node_from_list_at_index(string_list **, unsigned int);
void free_string_list(string_list **);

size_t list_length(const string_list *);
char **convert_list_to_strings(string_list *);
size_t print_list(const string_list *);
string_list *find_node_starts_with(string_list *, char *, char);
ssize_t get_node_index_in_list(string_list *, string_list *);

int is_command_chained(info_t *, char *, size_t *);
void check_command_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias_with_command(info_t *);
int replace_environment_variables(info_t *);
int replace_substring(char **, char *);

#endif /* _CUSTOM_SHELL_H_ */

