#include "custom_shell.h"

/**
 * Display command history
 * @info: Structure containing potential arguments.
 *
 * Returns: Always 0.
 */
int custom_history(info_t *info)
{
    print_string_list(info->command_history);
    return 0;
}

/**
 * Unset an alias
 * @info: Structure containing potential arguments.
 * @str: the string alias
 *
 * Returns: Always 0 on success, 1 on error.
 */
int unset_custom_alias(info_t *info, char *str)
{
    char *position, character;
    int result;

    position = string_find_character(str, '=');
    if (!position)
        return 1;
    character = *position;
    *position = '\0';
    result = delete_node_from_list_at_index(&(info->alias_list), get_node_index_in_list(info->alias_list, find_node_starts_with(info->alias_list, str, -1)));
    *position = character;
     result;
}

/**
 * Mimics the alias builtin
 * @info: Structure containing potential arguments.
 *
 * Returns: Always 0.
 */
int custom_alias(info_t *info)
{
    int i = 0;
    char *position = NULL;
    string_list *node = NULL;

    if (info->argument_count == 1)
    {
        node = info->alias_list;
        while (node)
        {
            print_custom_alias(node);
            node = node->next;
        }
        return 0;
    }
    for (i = 1; info->argument_vector[i]; i++)
    {
        position = string_find_character(info->argument_vector[i], '=');
        if (position)
            set_custom_alias(info, info->argument_vector[i]);
        else
            print_custom_alias(find_node_starts_with(info->alias_list, info->argument_vector[i], '='));
    }

    return 0;
}

