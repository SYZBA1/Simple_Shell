#include "custom_shell.h"

/**
 * Exit the shell
 * @info: Structure containing potential arguments.
 *
 * Returns: -2 if the exit code is provided, else -2.
 */
int custom_exit(info_t *info)
{
    int exit_code;

    if (info->argv[1]) {
        exit_code = _erratoi(info->argv[1]);
        if (exit_code == -1) {
            info->status = 2;
            print_error_message("Illegal number: ");
            print_string(info->argv[1]);
            print_character('\n');
            return 1;
        }
        info->error_number = _erratoi(info->argv[1]);
        return -2;
    }
    info->error_number = -1;
    return -2;
}

/**
 * Change the current directory of the process
 * @info: Structure containing potential arguments.
 *
 * Returns: Always 0.
 */
int custom_cd(info_t *info)
{
    char *current_directory, *new_directory, buffer[1024];
    int change_directory_result;

    current_directory = getcwd(buffer, 1024);
    if (!current_directory)
        print_error_message("TODO: >>getcwd failure emsg here<<\n");
    if (!info->argv[1]) {
        new_directory = _get_environment_variable(info, "HOME=");
        if (!new_directory)
            change_directory_result = chdir((new_directory = _get_environment_variable(info, "PWD=")) ? new_directory : "/");
        else
            change_directory_result = chdir(new_directory);
    } else if (string_compare(info->argv[1], "-") == 0) {
        if (!_get_environment_variable(info, "OLDPWD=")) {
            print_string(current_directory);
            print_character('\n');
            return 1;
        }
        print_string(_get_environment_variable(info, "OLDPWD="));
        print_character('\n');
        change_directory_result = chdir((new_directory = _get_environment_variable(info, "OLDPWD=")) ? new_directory : "/");
    } else {
        change_directory_result = chdir(info->argv[1]);
    }
    if (change_directory_result == -1) {
        print_error_message("can't cd to ");
        print_string(info->argv[1]);
        print_character('\n');
    } else {
        _set_environment_variable(info, "OLDPWD", _get_environment_variable(info, "PWD="));
        _set_environment_variable(info, "PWD", getcwd(buffer, 1024));
    }
    return 0;
}

/**
 * Display help information
 * @info: Structure containing potential arguments.
 *
 * Returns: Always 0.
 */
int custom_help(info_t *info)
{
    char **argument_array;

    argument_array = info->argument_vector;
    print_string("help call works. Function not yet implemented \n");
    if (0)
        print_string(*argument_array); /* temp att_unused workaround */
    return 0;
}

