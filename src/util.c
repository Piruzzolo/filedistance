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
#include "../include/util.h"

/**
 * Copy file contents character by charcter from
 * one file to another.
 * It return total character copied count.
 *
 * @sourceFile  Pointer to source FILE.
 * @destFile    Pointer to destination FILE.
 */
int filecpy(FILE * sourceFile, FILE * destFile)
{
    int  count = 0;
    char ch;

    /* Copy file contents character by character. */
    while ((ch = fgetc(sourceFile)) != EOF)
    {
        fputc(ch, destFile);

        /* Increment character copied count */
        count++;
    }

    return count;
}

int load_file(char* filename, char** buffer)
{
    int size = 0;

    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        *buffer = NULL;
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size = (int) ftell(f);

    fseek(f, 0, SEEK_SET);

    *buffer = (char*) malloc(size + 1);

    if (size != fread(*buffer, sizeof(char), size, f))
    {
        free(*buffer);
        return -2;
    }
    fclose(f);

    (*buffer)[size] = 0;

    return size;
}

int minmin(int x, int y, int z)
{
    int smallest = x;
    if (smallest > y) smallest = y;
    if (smallest > z) smallest = z;

    return smallest;
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


int min(int x, int y)
{
    return (x < y) ? x : y;
}

int max(int x, int y)
{
    return (x > y) ? x : y;
}