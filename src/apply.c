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

#include "../include/script.h"

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

int apply(char* infile, char* scriptfile, char* outfile)
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

    unsigned char buffer[4096];
    size_t sz = 0;

    while ((sz = fread(buffer, 1, sizeof(buffer), script)) > 0)
    {
        edit got;

        if (try_parse_ADD(&got, buffer))
        {

        }
        else if (try_parse_DEL(&got, buffer))
        {

        }
        else if (try_parse_SET(&got, buffer))
        {

        }
        else
        {

        }
    }






    fclose(in);
    fclose(script);
    fclose(out);

}