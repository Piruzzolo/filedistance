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

// node* list = NULL;

_Bool try_parse_ADD(edit *pEdit, char* buffer)
{
    return 0;
}

_Bool try_parse_DEL(edit *pEdit, char* buffer)
{
    return 0;
}

_Bool try_parse_SET(edit *pEdit, char* buffer)
{
    return 0;
}

int apply(const char* infile, const char* scriptfile, const char* outfile)
{
    if (infile == NULL || scriptfile == NULL || outfile == NULL)
    {
        return -1;
    }

    FILE* in  = fopen(infile, "r");
    FILE* script = fopen(scriptfile, "r");
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

    char opcode[4];
    opcode[3] = '\0';

    int read = fscanf(script, "%3s", opcode);
    if (!read)
    {
        //error...
    }

    if (strcmp(opcode, "ADD") == 0)
    {
        unsigned int n = 0;
        char b = 0;
        if (fscanf(script, "%ui", &n)) // check
        {

            if (fscanf(script, "%c", &b))
            {

            }
        }
    }
    else if (strcmp(opcode, "DEL") == 0)
    {
        fread(opcode, 1, 1, script); // todo
    }
    else if (strcmp(opcode, "SET") == 0)
    {

    } else
    {
        // error
    }

    fclose(in);
    fclose(script);
    fclose(out);

}