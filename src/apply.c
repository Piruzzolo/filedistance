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
#include <errno.h>

#include "../include/script.h"
#include "../include/list.h"
#include "../include/endianness.h"
#include "../include/util.h"
#include "../include/apply.h"

#define BUFSIZE 256
#define MAX_SIZE_ITEM 8


void apply_print_err(int err)
{
    switch (err)
    {
        case EEMPTYSCRIPT:
        {
            printf("ERROR: Script file is empty.\n");
            break;
        }
        case ECANTOPEN:
        {
            printf("ERROR: Can't open one or more files.\n");
            break;
        }
        case ECORRUPTD:
        {
            printf("ERROR: Script file is invalid or corrupted.\n");
            break;
        }

        default:
            return;
    }
}


u_int32_t bytes_to_uint32(char* buf)
{
    return buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
}


bool parse_ADD(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM];
    fread(buf, MAX_SIZE_ITEM, 1, file);// todo check fread read bytes
    if (strncmp(buf, "ADD", 3) == 0)
    {
        result->operation = ADD;
        result->pos = ntohl(bytes_to_uint32(&buf[3]));
        result->c = buf[MAX_SIZE_ITEM - 1];

        return true;
    }

    fseek(file, p, SEEK_SET);
    return false;
}

bool parse_DEL(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM - 1];
    fread(buf, MAX_SIZE_ITEM - 1, 1, file);// todo check fread read bytes
    if (strncmp(buf, "DEL", 3) == 0)
    {
        result->operation = DEL;
        result->pos = ntohl(bytes_to_uint32(&buf[3]));

        return true;
    }

    fseek(file, p, SEEK_SET);
    return false;
}

bool parse_SET(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL || p == EOF)
    {
        return false;
    }

    char buf[MAX_SIZE_ITEM];

    fread(buf, MAX_SIZE_ITEM, 1, file); // todo check fread read bytes
    if (strncmp(buf, "SET", 3) == 0)
    {
        result->operation = SET;
        result->pos = ntohl(bytes_to_uint32(&buf[3]));
        result->c = buf[MAX_SIZE_ITEM - 1];

        return true;
    }

    fseek(file, p, SEEK_SET);
    return false;
}

void apply_ADD(FILE* out, edit* toApply)
{
    char c = toApply->c;
    fputc(c, out);
}

void apply_DEL(FILE* in, edit* toApply)
{
    fseek(in, toApply->pos + 1, SEEK_SET);
}

void apply_SET(FILE* out, FILE* in, edit* toApply)
{
    fseek(in, toApply->pos + 1, SEEK_SET);
    char b = toApply->c;
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

    /* open infile and scriptfile read, open outfile write */
    FILE* in         = fopen(infile,  "r");
    FILE* scriptfile = fopen(filem,   "r");
    FILE* out        = fopen(outfile, "w");

    if (in == NULL || scriptfile == NULL || out == NULL)
    {
        errno = ECANTOPEN;
        return -1;
    }

    /* fail if script file size == 0 */
    struct stat st;
    stat(filem, &st);
    if (st.st_size == 0)
    {
        errno = EEMPTYSCRIPT;
        return -1;
    }

    /* get size of infile */
    struct stat st2;
    stat(infile, &st2);
    int size_infile = st2.st_size;

    int numOps = get_num_ops(scriptfile) - 1;

    /* ensure seeks are at a known value, begin */
    rewind(in);
    rewind(scriptfile);
    rewind(out);

    edit todo;

    for (int i = 0; i < 20; i++) // todo get_num_ops o gestire con un while...
    //while (!feof(scriptfile))
    {
        /* try to parse the command, save the result in todo,
         * copy bytes from last position to todo's position
         * then apply the operation read */

        if (parse_ADD(scriptfile, &todo))
        {
            file_copy(in, out, todo.pos - ftell(in) + 1);
            apply_ADD(out, &todo);
        }
        else if (parse_DEL(scriptfile, &todo))
        {
            file_copy(in, out, todo.pos - ftell(in));
            apply_DEL(in, &todo);
        }
        else if (parse_SET(scriptfile, &todo))
        {
            file_copy(in, out, todo.pos - ftell(in));
            apply_SET(out, in, &todo);
        }
        else
        {
            errno = ECORRUPTD;
            return -1;
        }

        /* clean up todo for next cycle */
        memset(&todo, 0, sizeof(todo));
    }

    /* copy from infile's seek its EOF */
    file_copy(in, out, size_infile - ftell(in));

    /* close files */
    fclose(in);
    fclose(scriptfile);
    fclose(out);

    return 0;
}