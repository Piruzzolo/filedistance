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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "../include/script.h"
#include "../include/list.h"
#include "../include/endianness.h"
#include "../include/util.h"

#define BUFSIZE 256
#define MAX_SIZE_ITEM 8

u_int32_t bytes_to_uint32(char* buf)
{
    return buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
}

_Bool parse_ADD(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM];
    fread(buf, MAX_SIZE_ITEM, 1, file);
    if (strncmp(buf, "ADD", 3) == 0)
    {
        result->operation = ADD;
        result->pos = ntohl(bytes_to_uint32(&buf[3]));
        result->arg2 = buf[MAX_SIZE_ITEM-1];

        return true;
    }

    fseek(file, p, SEEK_SET);
    return false;
}

_Bool parse_DEL(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM - 1];
    fread(buf, MAX_SIZE_ITEM - 1, 1, file);
    if (strncmp(buf, "DEL", 3) == 0)
    {
        result->operation = DEL;
        result->pos = ntohl(bytes_to_uint32(&buf[3]));

        return true;
    }

    fseek(file, p, SEEK_SET);
    return false;
}

_Bool parse_SET(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM];
    fread(buf, MAX_SIZE_ITEM, 1, file);
    if (strncmp(buf, "SET", 3) == 0)
    {
        result->operation = SET;
        result->pos = ntohl(bytes_to_uint32(&buf[3]));
        result->arg2 = buf[MAX_SIZE_ITEM-1];

        return true;
    }

    fseek(file, p, SEEK_SET);
    return false;
}

_Bool apply_ADD(FILE* out, edit* toApply)
{
    char b = toApply->arg2;
    fputc(b, out);
}

_Bool apply_DEL(FILE* in)
{
    fseek(in, 1, SEEK_CUR);
}

_Bool apply_SET(FILE* out, FILE* in, edit* toApply)
{
    fseek(in, 1, SEEK_CUR);
    char b = toApply->arg2;
    fputc(b, out);
}

int get_num_ops(FILE* script)
{
    return count_occurrences(script, "ADD") +
           count_occurrences(script, "DEL") +
           count_occurrences(script, "SET");
}

int apply_edit_script(const char* infile, const char* filem, const char* outfile)
{
    if (infile == NULL || filem == NULL || outfile == NULL)
    {
        return -1;
    }

    FILE* in  = fopen(infile, "r");
    FILE* scriptfile = fopen(filem, "r");
    FILE* out = fopen(outfile, "w");
    if (in == NULL)
    {
        perror("Can't open infile");
        return -1;
    }
    if (in == NULL)
    {
        perror("Can't open script");
        return -1;
    }
    if (out == NULL)
    {
        perror("Can't open outfile");
        return -1;
    }

    struct stat st;
    stat(filem, &st);
    if (st.st_size == 0)
    {
        return -1;
    }

    int numOps = get_num_ops(scriptfile);

    for (int i = 0; i < numOps; i++)
    {
        edit todo;

        if (parse_ADD(scriptfile, &todo))
        {
            file_copy(in, out, todo.pos - ftell(in));
            apply_ADD(out, &todo);
        }
        else if (parse_DEL(scriptfile, &todo))
        {
            file_copy(in, out, todo.pos - ftell(in));
            apply_DEL(in);
        }
        else if (parse_SET(scriptfile, &todo))
        {
            file_copy(in, out, todo.pos - ftell(in));
            apply_SET(out, in, &todo);
        }
        else
        {
            // vedere
        }

        memset(&todo, 0, sizeof(todo));
    }

    if (in)         fclose(in);
    if (scriptfile) fclose(scriptfile);
    if (out)        fclose(out);

    return 0;
}
