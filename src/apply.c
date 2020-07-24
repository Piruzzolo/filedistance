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
#include <sys/stat.h>
#include <errno.h>

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


int apply_edit_script(const char* infilename, const char* scriptfilename, const char* outfilename)
{
    if (infilename == NULL || scriptfilename == NULL || outfilename == NULL)
    {
        return -1;
    }

    /* open infile and scriptfile read, open outfile write */

    FILE* infile     = fopen(infilename,  "r");
    FILE* scriptfile = fopen(scriptfilename,   "r");
    FILE* outfile    = fopen(outfilename, "w");

    if (!infile || !scriptfile || !outfile)
    {
        errno = ECANTOPEN;
        return -1;
    }

    /* fail if script file size == 0 */

    struct stat st1;
    stat(scriptfilename, &st1);
    if (st1.st_size == 0)
    {
        errno = EEMPTYSCRIPT;
        return -1;
    }

    /* ensure seeks are at a known value, begin */

    rewind(infile);
    rewind(scriptfile);
    rewind(outfile);

    char buf[CMDSIZE];

    while (fread(&buf, CMDSIZE, 1, scriptfile) > 0)
    {
        /* get command's position and char */
        u_int32_t position = ntohl(bytes_to_uint32(&buf[3]));
        char c = buf[CMDSIZE - 1];

        if (strncmp(buf, "ADD", 3) == 0)
        {
            /* copy from infile's seek to position + 1 */
            file_copy_to(infile, outfile, position + 1);
            fputc(c, outfile);
        }
        else if (strncmp(buf, "DEL", 3) == 0)
        {
            file_copy_to(infile, outfile, position);
            fseek(infile, 1, SEEK_CUR);
        }
        else if (strncmp(buf, "SET", 3) == 0)
        {
            file_copy_to(infile, outfile, position + 1);
            fseek(outfile, -1, SEEK_CUR);
            fputc(c, outfile);
        }
        else
        {
            errno = ECORRUPTD;
            return -1;
        }
    }

    /* copy every other char */
    file_copy(infile, outfile);

    /* close files */
    fclose(infile);
    fclose(scriptfile);
    fclose(outfile);

    return 0;
}