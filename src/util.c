/* This file is part of FileDistance
*  Copyright (C) 2020  Marco Savelli
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strlen
#include <stdbool.h>

#include "../include/util.h"


int minmin(int x, int y, int z)
{
    return min(min(x, y), z);
}


void swap_array_edit(edit* a, edit* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        edit tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
    }
}


void swap_array_int( int* a, int* b, size_t n )
{
    for ( size_t i = 0; i < n; i++ )
    {
        int tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
    }
}


int count_occurrences(FILE* file, const char* word)
{
    int count = 0;
    int ch = 0;
    int len = strlen(word);

    while (true)
    {
        if ((ch = fgetc(file)) == EOF)
            break;

        if ((char) ch != *word)
            continue;

        for (int i = 1; i < len; ++i)
        {
            if ((ch = fgetc(file)) == EOF)
                return count;

            if ((char) ch != word[i])
            {
                fseek(file, 1 - i, SEEK_CUR);
                continue;
            }
        }
        ++count;
    }

    rewind(file);
    return count;
}


int file_copy(FILE* in, FILE* out, unsigned int len)
{
    char c;
    for (int i = 0; i < len; i++)
    {
        c = getc(in);
        if (c == EOF) return -1;
        putc(c, out);
    }
    return 0;
}


int min(int x, int y)
{
    return (x < y) ? x : y;
}


int max(int x, int y)
{
    return (x > y) ? x : y;
}