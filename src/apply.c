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

_Bool parse_ADD(char* buffer, edit* result)
{
    if (strncmp(buffer, "ADD", 3) == 0)
    {

    }
    return false;
}

_Bool parse_DEL(char* buffer, edit* result)
{
    return 0;
}

_Bool parse_SET(char* buffer, edit* result)
{
    return 0;
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

        if (strlen(buf) == MAX_SIZE_ITEM)
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

    if (in)         fclose(in);
    if (scriptfile) fclose(scriptfile);
    if (out)        fclose(out);

}
