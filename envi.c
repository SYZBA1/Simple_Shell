#include "custom_shell.h"

/**
 * Print the current environment
 * @info: Structure containing potential arguments.
 * Returns: Always 0
 */
int print_environment(info_t *info)
{
    print_list_str(info->environment);
    return 0;
}

/**
 * Gets the value of an environment variable
 * @info: Structure containing potential arguments.
 * @name: Environment variable name
 * Returns: The value of the environment variable
 */
char *get_environment(info_t *info, const char *name)
{
    list_t *node = info->environment;
    char *p;

    while (node)
    {
        p = starts_with(node->str, name);
        if (p && *p)
            return p;
        node = node->next;
    }
    return NULL;
}

/**
 * Initialize a new environment variable, or modify an existing one
 * @info: Structure containing potential arguments.
 * Returns: Always 0
 */
int initialize_environment_variable(info_t *info)
{
    if (info->argc != 3)
    {
        print_string("Incorrect number of arguments\n");
        return 1;
    }
    if (set_environment_variable(info, info->argv[1], info->argv[2]))
        return 0;
    return 1;
}

/**
 * Remove an environment variable
 * @info: Structure containing potential arguments.
 * Returns: Always 0
 */
int remove_environment_variable(info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        print_string("Too few arguments.\n");
        return 1;
    }
    for (i = 1; i <= info->argc; i++)
        unset_environment_variable(info, info->argv[i]);

    return 0;
}

/**
 * Populates the environment linked list
 * @info: Structure containing potential arguments.
 * Returns: Always 0
 */
int populate_environment_list(info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
        add_node_end(&node, environ[i], 0);
    info->environment = node;
    return 0;
}

