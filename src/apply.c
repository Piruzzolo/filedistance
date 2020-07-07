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

#include "../include/script.h"
#include "../include/list.h"
#include <stdbool.h>

// glibc doesn't have strlcpy
#ifdef __GNU_LIBRARY__
#include "../include/safe_str/strlcpy.h"
#endif

#define BUFSIZE 256
#define MAX_SIZE_ITEM 8

u_int32_t bytes_to_uint32(unsigned char* buf)
{
    return buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
}

_Bool parse_ADD(FILE* file, edit* result)
{
    edit* tmp = (edit*) malloc(sizeof(edit));
    if (result == NULL || !tmp)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM];
    fread(buf, MAX_SIZE_ITEM, 1, file);
    if (strncmp(buf, "ADD", 3) == 0)
    {
        tmp->operation = ADD;
        tmp->pos = bytes_to_uint32(&buf[3]);
        tmp->arg2 = buf[MAX_SIZE_ITEM-1];

        result = tmp;
        return true;
    }
    return false;
}

_Bool parse_DEL(FILE* file, edit* result)
{
    edit* tmp = (edit*) malloc(sizeof(edit));
    if (result == NULL || !tmp)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM - 1];
    fread(buf, MAX_SIZE_ITEM - 1, 1, file);
    if (strncmp(buf, "DEL", 3) == 0)
    {
        tmp->operation = DEL;
        tmp->pos = bytes_to_uint32(buf + 3);

        result = tmp;
        return true;
    }

    return false;
}

_Bool parse_SET(FILE* file, edit* result)
{
    edit* tmp = (edit*) malloc(sizeof(edit));
    if (result == NULL || !tmp)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM];
    fread(buf, MAX_SIZE_ITEM, 1, file);
    if (strncmp(buf, "SET", 3) == 0)
    {
        tmp->operation = ADD;
        tmp->pos = bytes_to_uint32(&buf[3]);
        tmp->arg2 = buf[MAX_SIZE_ITEM-1];

        result = tmp;
        return true;
    }

    return false;
}

_Bool apply_ADD(FILE* out, edit* toApply)
{
    char b = toApply->arg2;
    fputc(b, out);
}

_Bool apply_DEL(FILE* out, edit* toApply)
{
    fseek(out, -1, SEEK_CUR);
}

_Bool apply_SET(FILE* out, FILE* in, edit* toApply) // todo: ALL: check for EOF
{
    char b = toApply->arg2;
    fseek(in, 1, SEEK_CUR);
    fputc(b, out);
    fseek(out, 1, SEEK_CUR);
}

int apply(const char* infile, const char* filem, const char* outfile)
{
    if (infile == NULL || filem == NULL || outfile == NULL)
    {
        return -1;
    }

    FILE* in  = fopen(infile, "r");
    FILE* scriptfile = fopen(filem, "r");
    FILE* out = fopen(infile, "wb");
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

    _Bool res = 0;

    char buf[MAX_SIZE_ITEM];

    while (fgets(buf, MAX_SIZE_ITEM, scriptfile))
    {
        edit todo;

        if (strlen(buf) == MAX_SIZE_ITEM) // TODO reformat into list of edits
        {
            if (parse_ADD(buf, &todo))
            {
                res &= apply_ADD(out, &todo);
            }
            else if (parse_DEL(buf, &todo))
            {
                res &= apply_DEL(out, &todo);
            }
            else if (parse_SET(buf, &todo))
            {
                res &= apply_SET(out, in, &todo);
            }
            else
            {
                // vedere
            }



        }
        else
        {
            // vedere
        }
    }

    node* list = NULL;

    while (list->next)
    {

    }







    if (in)         fclose(in);
    if (scriptfile) fclose(scriptfile);
    if (out)        fclose(out);

}
