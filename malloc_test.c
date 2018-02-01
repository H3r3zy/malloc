
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _stringify(a) #a
#define stringify(a) _stringify(a)
#define MIN(a,b)     ((a) < (b) ? (a) : (b))

int nbr(int value, char *base)
{
    int len;
    int neg_flag;

    len = -1;
    neg_flag = 1;
    while (*(base + ++len));
    if (len < 2)
        return -1;
    if (value < 0)
    {
        write(1, "-", 1);
        neg_flag = -1;
    }
    if (value / len)
        nbr((value / len) * neg_flag, base);
    write(1, &base[(value % len) * neg_flag], 1);
    return value;
}
void hex(int value)
{
    nbr(value, "0123456789ABCDEF");
}
void dec(int value)
{
    nbr(value, "0123456789");
}
void str(char *str)
{
    write(1, str, strlen(str));
}

void stress()
{
#define LENGTH 4096
#define LENGTH_AS_STRING stringify(LENGTH)
    char *pointers[LENGTH];
    int sizes[LENGTH];
    int malloked[LENGTH];
    int i, offset, size;

    memset(malloked, 0, LENGTH * sizeof(int));
    str("char *pointers[" LENGTH_AS_STRING "];\n");
    while (1)
    {
        offset = rand() % LENGTH;
        size = rand() % 666;
        if (malloked[offset])
        {
            for (i = 0; i < sizes[offset]; i++)
                if (pointers[offset][i] != offset % 127)
                    {
                        str("Memory corruption at pointers[");dec(offset);str("][");dec(i);str("]\n");
                        exit(EXIT_FAILURE);
                    }
            if (rand() % 2 == 0)
            {
                str("free(pointers[");dec(offset);str("]);\n");
                free(pointers[offset]);
                malloked[offset] = 0;
            }
            else
            {
                str("pointers[");dec(offset);str("] = realloc(");str("pointers[");dec(offset);str("], ");dec(size);str(");\n");
                if (malloked[offset])
                {
                    pointers[offset] = realloc(pointers[offset], size);
                    for (i = 0; i < MIN(size, sizes[offset]) ; i++)
                        if (pointers[offset][i] != offset % 127)
                        {
                            str("Memory corruption, in your realloc at pointers[");dec(offset);str("][");dec(i);str("]\n");
                            exit(EXIT_FAILURE);
                        }
                    memset(pointers[offset], offset % 127, size);
                    sizes[offset] = size;
                }
            }
        }
        else
        {
            str("pointers[");dec(offset);str("] = malloc(");dec(size);str(");\n");
            pointers[offset] = malloc(size);
            malloked[offset] = 1;
            sizes[offset] = size;
            memset(pointers[offset], offset % 127, size);
        }
    }
}

int main(int ac, char **av)
{
    if (ac == 1)
    {
#define USAGE "Missing argument: please provide a seed\n" \
        "The seed make your test reproductible:\n" \
        "Giving the same seed will run the same tests.\n"
        write(2, USAGE, strlen(USAGE));
        return EXIT_FAILURE;
    }
    srand(atoi(av[1]));
    stress();
    return EXIT_SUCCESS;
}
