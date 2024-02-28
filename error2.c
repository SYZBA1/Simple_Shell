#include "custom_shell.h"

/**
 * Converts a string to an integer
 * @s: The string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int convert_string_to_int(char *s)
{
    int i = 0;
    unsigned long int result = 0;

    if (*s == '+')
        s++;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            result *= 10;
            result += (s[i] - '0');
            if (result > INT_MAX)
                return -1;
        }
        else
            return -1;
    }
    return (int)result;
}

/**
 * Prints an error message
 * @info: The parameter & return info struct
 * @estr: String containing specified error type
 * Return: None
 */
void print_error(info_t *info, char *estr)
{
    print_string_fd(info->fname, STDERR_FILENO);
    print_character_fd(':', STDERR_FILENO);
    print_integer_fd(info->line_count, STDERR_FILENO);
    print_string_fd(":", STDERR_FILENO);
    print_string_fd(info->argv[0], STDERR_FILENO);
    print_string_fd(":", STDERR_FILENO);
    print_string_fd(estr, STDERR_FILENO);
}

/**
 * Prints a decimal (integer) number (base 10)
 * @input: The input number
 * @fd: The file descriptor to write to
 * Return: The number of characters printed
 */
int print_integer_fd(int input, int fd)
{
    int (*putchar_fd)(char, int) = print_character_fd;
    int i, count = 0;
    unsigned int absValue, current;

    if (fd != STDERR_FILENO)
        putchar_fd = print_character_fd;

    if (input < 0)
    {
        absValue = -input;
        putchar_fd('-', fd);
        count++;
    }
    else
    {
        absValue = input;
    }

    current = absValue;
    for (i = 1000000000; i > 1; i /= 10)
    {
        if (absValue / i)
        {
            putchar_fd('0' + current / i, fd);
            count++;
        }
        current %= i;
    }
    putchar_fd('0' + current, fd);
    count++;

    return count;
}

/**
 * Converter function, a clone of itoa
 * @num: The number to convert
 * @base: The base for conversion
 * @flags: Argument flags
 * Return: The converted string
 */
char *convert_number(long int num, int base, int flags)
{
    static char buffer[50];
    char *array = (flags & CONVERT_LOWERCASE) ? "0123456789abcdef" : "0123456789ABCDEF";
    char sign = 0;
    char *ptr;
    unsigned long n = (num < 0 && !(flags & CONVERT_UNSIGNED)) ? -num : num;

    ptr = &buffer[49];
    *ptr = '\0';

    if (num < 0 && !(flags & CONVERT_UNSIGNED))
    {
        n = -num;
        sign = '-';
    }

    do
    {
        *--ptr = array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;

    return ptr;
}

/**
 * Function replaces the first instance of '#' with '\0'
 * @buf: Address of the string to modify
 * Return: Always 0
 */
void remove_comments(char *buf)
{
    int i;

    for (i = 0; buf[i] != '\0'; i++)
    {
        if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
        {
            buf[i] = '\0';
            break;
        }
    }
}

