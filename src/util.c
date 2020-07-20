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

#include <stdlib.h>
#include <stdio.h>

#include "../include/util.h"


int min(int x, int y)
{
    return (x < y) ? x : y;
}


int minmin(int x, int y, int z)
{
    return min(min(x, y), z);
}


int file_copy(FILE* in, FILE* out, unsigned int len)
{
    if (len == 0)
        return 0;

    char c;
    for (int i = 0; i < len; i++)
    {
        c = (char) getc(in);
        if (c == EOF)
            return -1;
        putc(c, out);
    }

    return 0;
}

u_int32_t bytes_to_uint32(char* buf)
{
    return buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
}
