#include "custom_shell.h"

/**
 * Returns the string array copy of our environ
 * @info: Structure containing potential arguments.
 * Return: String array copy of the environment
 */
char **get_environment(info_t *info)
{
    if (!info->environment || info->environment_changed)
    {
        info->environment = list_to_strings(info->env);
        info->environment_changed = 0;
    }

    return (info->environment);
}

/**
 * Remove an environment variable
 * @info: Structure containing potential arguments.
 * @var: The string env var property
 * Return: 1 on delete, 0 otherwise
 */
int remove_environment_variable(info_t *info, char *var)
{
    list_t *node = info->env;
    size_t index = 0;
    char *p;

    if (!node || !var)
        return 0;

    while (node)
    {
        p = starts_with(node->str, var);
        if (p && *p == '=')
        {
            info->environment_changed = delete_node_at_index(&(info->env), index);
            index = 0;
            node = info->env;
            continue;
        }
        node = node->next;
        index++;
    }
    return info->environment_changed;
}

/**
 * Initialize a new environment variable or modify an existing one
 * @info: Structure containing potential arguments.
 * @var: The string env var property
 * @value: The string env var value
 * Return: Always 0
 */
int set_environment_variable(info_t *info, char *var, char *value)
{
    char *buf = NULL;
    list_t *node;
    char *p;

    if (!var || !value)
        return 0;

    buf = malloc(_strlen(var) + _strlen(value) + 2);
    if (!buf)
        return 1;
    _strcpy(buf, var);
    _strcat(buf, "=");
    _strcat(buf, value);
    node = info->env;
    while (node)
    {
        p = starts_with(node->str, var);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buf;
            info->environment_changed = 1;
            return 0;
        }
        node = node->next;
    }
    add_node_end(&(info->env), buf, 0);
    free(buf);
    info->environment_changed = 1;
    return 0;
}

