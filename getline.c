#include "custom_shell.h"

/**
 * Buffer chained commands
 * @info: Structure containing potential arguments.
 * @buf: Address of buffer.
 * @len: Address of length variable.
 *
 * Returns: Bytes read.
 */
ssize_t buffer_commands(info_t *info, char **buf, size_t *len)
{
    ssize_t bytes_read = 0;
    size_t len_p = 0;

    if (!*len) { /* If nothing left in the buffer, fill it */
        free(*buf);
        *buf = NULL;
        signal(SIGINT, sigint_handler);
#if USE_GETLINE
        bytes_read = getline(buf, &len_p, stdin);
#else
        bytes_read = custom_getline(info, buf, &len_p);
#endif
        if (bytes_read > 0) {
            if ((*buf)[bytes_read - 1] == '\n') {
                (*buf)[bytes_read - 1] = '\0'; /* Remove trailing newline */
                bytes_read--;
            }
            info->linecount_flag = 1;
            remove_comments(*buf);
            build_history_list(info, *buf, info->histcount++);
            /* Check if this is a command chain */
            {
                *len = bytes_read;
                info->command_buffer = buf;
            }
        }
    }
    return bytes_read;
}

/**
 * Get a line of input from STDIN
 * @info: Structure containing potential arguments.
 *
 * Returns: Bytes read.
 */
ssize_t get_input(info_t *info)
{
    static char *buf; /* The ';' command chain buffer */
    static size_t i, j, len;
    ssize_t bytes_read = 0;
    char **buf_p = &(info->arg), *p;

    print_character(BUFFER_FLUSH);
    bytes_read = buffer_commands(info, &buf, &len);
    if (bytes_read == -1) /* EOF */
        return -1;
    if (len) { /* We have commands left in the chain buffer */
        j = i; /* Initialize new iterator to current buf position */
        p = buf + i; /* Get pointer for return */

        check_chain(info, buf, &j, i, len);
        while (j < len) { /* Iterate to semicolon or end */
            if (is_chain(info, buf, &j))
                break;
            j++;
        }

        i = j + 1; /* Increment past null terminator */
        if (i >= len) /* Reached end of buffer? */
        {
            i = len = 0; /* Reset position and length */
            info->command_buffer_type = CMD_NORM;
        }

        *buf_p = p; /* Pass back pointer to current command position */
        return string_length(p); /* Return length of current command */
    }

    *buf_p = buf; /* Else not a chain, pass back buffer from custom_getline() */
    return bytes_read; /* Return length of buffer from custom_getline() */
}

/**
 * Read a buffer
 * @info: Structure containing potential arguments.
 * @buf: Buffer.
 * @i: Size.
 *
 * Returns: r.
 */
ssize_t read_buffer(info_t *info, char *buf, size_t *i)
{
    ssize_t bytes_read = 0;

    if (*i)
        return 0;
    bytes_read = read(info->read_file_descriptor, buf, READ_BUF_SIZE);
    if (bytes_read >= 0)
        *i = bytes_read;
    return bytes_read;
}

/**
 * Get the next line of input from STDIN
 * @info: Structure containing potential arguments.
 * @ptr: Address of pointer to buffer, preallocated or NULL.
 * @length: Size of preallocated ptr buffer if not NULL.
 *
 * Returns: s.
 */
int custom_getline(info_t *info, char **ptr, size_t *length)
{
    static char buf[READ_BUF_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length)
        s = *length;
    if (i == len)
        i = len = 0;

    r = read_buffer(info, buf, &len);
    if (r == -1 || (r == 0 && len == 0))
        return -1;

    c = string_find_character(buf + i, '\n');
    k = c ? 1 + (size_t)(c - buf) : len;
    new_p = custom_realloc(p, s, s ? s + k : k + 1);
    if (!new_p) /* MALLOC FAILURE! */
        return (p ? free(p), -1 : -1);

    if (s)
        string_ncat(new_p, buf + i, k - i);
    else
        string_ncpy(new_p, buf + i, k - i + 1);

    s += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = s;
    *ptr = p;
    return s;
}

/**
 * Signal handler for SIGINT
 * @sig_num: The signal number
 *
 * Returns: void.
 */
void sigint_handler(__attribute__((unused))int sig_num)
{
    print_string("\n");
    print_string("$ ");
    print_character(BUFFER_FLUSH);
}

