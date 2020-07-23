/* This file is part of filedistance
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

#define CMDSIZE 8

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


bool parse_ADD(FILE* file, edit* result)
{
    long p = ftell(file);
    if (result == NULL)
    {
        return false;
    }

    char buf[CMDSIZE];
    if (fread(buf, 1, CMDSIZE, file) == CMDSIZE)
    {
        if (strncmp(buf, "ADD", 3) == 0)
        {
            result->operation = ADD;
            result->position = ntohl(bytes_to_uint32(&buf[3]));
            result->c = buf[CMDSIZE - 1];

            return true;
        }
    }
    else
    {
        return false;
    }

    /* rollback seek */
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

    char buf[CMDSIZE];
    if (fread(buf, 1, CMDSIZE, file) == CMDSIZE)
    {
        if (strncmp(buf, "DEL", 3) == 0)
        {
            result->operation = DEL;
            result->position = ntohl(bytes_to_uint32(&buf[3]));

            return true;
        }
    }
    else
    {
        return false;
    }

    /* rollback seek */
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

    char buf[CMDSIZE];

    if (fread(buf, 1, CMDSIZE, file) == CMDSIZE)
    {
        if (strncmp(buf, "SET", 3) == 0)
        {
            result->operation = SET;
            result->position = ntohl(bytes_to_uint32(&buf[3]));
            result->c = buf[CMDSIZE - 1];

            return true;
        }
    }
    else
    {
        return false;
    }

    /* rollback seek */
    fseek(file, p, SEEK_SET);
    return false;
}


void apply_ADD(FILE* out, edit* e)
{
    char c = e->c;
    fputc(c, out);
}


void apply_DEL(FILE* in, edit* e)
{
    fseek(in, e->position + 1, SEEK_SET);
}


void apply_SET(FILE* out, FILE* in, edit* e)
{
    fseek(in, e->position + 1, SEEK_SET);
    char b = e->c;
    fputc(b, out);
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

    if (!in || !scriptfile || !out)
    {
        errno = ECANTOPEN;
        return -1;
    }

    /* fail if script file size == 0 */

    struct stat st1;
    stat(filem, &st1);
    if (st1.st_size == 0)
    {
        errno = EEMPTYSCRIPT;
        return -1;
    }

    /* get size of infile */

    struct stat st2;
    stat(infile, &st2);
    int size_infile = st2.st_size;

    /* ensure seeks are at a known value, begin */

    //rewind(in);
    //rewind(scriptfile);
    //rewind(out);

    fseek(in, 0L, SEEK_SET);
    fseek(scriptfile, 0L, SEEK_SET);
    fseek(out, 0L, SEEK_SET);

    //edit cmd = {0};
    //edit last = {0};
//
    //int cnt = 0;
//
    //while (true)
    //{
    //    /* try to parse the command, save the result in cmd;
    //       copy bytes from last position to cmd's position
    //       then apply the operation read */
    //    if (parse_ADD(scriptfile, &cmd))
    //    {
    //        file_copy(in, out, abs(cmd.position - cnt - ftell(in)));
    //        apply_ADD(out, &cmd);
    //    }
    //    else if (parse_DEL(scriptfile, &cmd))
    //    {
    //        if (last.position == cmd.position && last.operation == cmd.operation)
    //            cnt++;
//
    //        file_copy(in, out, abs(cmd.position + cnt - ftell(in)));
    //        apply_DEL(in, &cmd);
    //    }
    //    else if (parse_SET(scriptfile, &cmd))
    //    {
    //        file_copy(in, out, abs(cmd.position - cnt - ftell(in)));
    //        apply_SET(out, in, &cmd);
    //    }
    //    else
    //    {
    //        if (!feof(scriptfile))
    //        {
    //            errno = ECORRUPTD;
    //            return -1;
    //        }
    //        break;
    //    }
    //    last = cmd;
//
    //    /* clean up cmd for next iteration */
    //    memset(&cmd, 0, sizeof(cmd));
    //}
    ///* copy from infile's seek till its EOF */
    //file_copy(in, out, size_infile - ftell(in));

    char buf[CMDSIZE];

    while (fread(&buf, CMDSIZE, 1, scriptfile) > 0)
    {
        u_int32_t position = ntohl(bytes_to_uint32(&buf[3]));
        char c = buf[CMDSIZE - 1];

        if (strncmp(buf, "ADD", 3) == 0)
        {
            file_copy(in, out, position);
            fseek(in, position, SEEK_SET);
            fputc(c, out);
        }
        else if (strncmp(buf, "DEL", 3) == 0)
        {
            file_copy(in, out, position);
            fseek(in, position + 1, SEEK_SET);
        }
        else if (strncmp(buf, "SET", 3) == 0)
        {
            file_copy(in, out, position);
            fseek(in, position + 1, SEEK_SET);
            fputc(c, out);
        }
        else
        {
            errno = ECORRUPTD;
            return -1;
        }
    }

    /* close files */
    fclose(in);
    fclose(scriptfile);
    fclose(out);

    return 0;
}