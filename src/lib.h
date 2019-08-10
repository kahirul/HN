#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *i_to_s(int i)
{

    char *str = malloc(sizeof(char) * 12);
    sprintf(str, "%d", i);

    return str;
}

char *time_to_s(int time, char *format)
{
    time_t rawtime = (time_t)time;
    struct tm *ts = localtime(&rawtime);

    size_t size = sizeof(char) * 80;
    char *str = malloc(size);

    strftime(str, size, format, ts);

    return str;
}
