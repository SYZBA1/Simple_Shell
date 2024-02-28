#include "custom_shell.h"

/**
 * Function to test if current character in buffer is a chain delimiter
 * @info: the parameter struct
 * @buf: the character buffer
 * @p: address of current position in buf
 *
 * Returns: 1 if chain delimiter, 0 otherwise
 */
int is_command_chained(info_t *info, char *buf, size_t *p)
{
    size_t j = *p;

    if (buf[j] == '|' && buf[j + 1] == '|')
    {
        buf[j] = 0;
        j++;
        info->command_buffer_type = COMMAND_OR;
    }
    else if (buf[j] == '&' && buf[j + 1] == '&')
    {
        buf[j] = 0;
        j++;
        info->command_buffer_type = COMMAND_AND;
    }
    else if (buf[j] == ';') // found end of this command
    {
        buf[j] = 0; // replace semicolon with null
        info->command_buffer_type = COMMAND_CHAIN;
    }
    else
        return 0;

    *p = j;
    return 1;
}

/**
 * Function to check whether to continue chaining based on last status
 * @info: the parameter struct
 * @buf: the character buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Returns: Void
 */
void check_command_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
    size_t j = *p;

    if (info->command_buffer_type == COMMAND_AND)
    {
        if (info->execution_status)
        {
            buf[i] = 0;
            j = len;
        }
    }
    if (info->command_buffer_type == COMMAND_OR)
    {
        if (!info->execution_status)
        {
            buf[i] = 0;
            j = len;
        }
    }

    *p = j;
}

/**
 * Function to replace aliases in the tokenized string
 * @info: the parameter struct
 *
 * Returns: 1 if replaced, 0 otherwise
 */
int replace_alias_with_command(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = find_node_starts_with(info->alias_list, info->argument_vector[0], '=');
        if (!node)
            return 0;
        free(info->argument_vector[0]);
        p = string_find_character(node->string, '=');
        if (!p)
            return 0;
        p = string_duplicate(p + 1);
        if (!p)
            return 0;
        info->argument_vector[0] = p;
    }
    return 1;
}

/**
 * Function to replace variables in the tokenized string
 * @info: the parameter struct
 *
 * Returns: 1 if replaced, 0 otherwise
 */
int replace_environment_variables(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->argument_vector[i]; i++)
    {
        if (info->argument_vector[i][0] != '$' || !info->argument_vector[i][1])
            continue;

        if (!string_compare(info->argument_vector[i], "$?"))
        {
            replace_string(&(info->argument_vector[i]), string_duplicate(convert_number(info->execution_status, 10, 0)));
            continue;
        }
        if (!string_compare(info->argument_vector[i], "$$"))
        {
            replace_string(&(info->argument_vector[i]), string_duplicate(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = find_node_starts_with(info->environment, &(info->argument_vector[i][1]), '=');
        if (node)
        {
            replace_string(&(info->argument_vector[i]), string_duplicate(string_find_character(node->string, '=') + 1));
            continue;
        }
        replace_string(&info->argument_vector[i], string_duplicate(""));
    }
    return 0;
}

/**
 * Function to replace string
 * @old: address of old string
 * @new: new string
 *
 * Returns: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
    free(*old);
    *old = new;
    return 1;
}

