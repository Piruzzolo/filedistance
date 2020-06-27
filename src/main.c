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
#include <errno.h>
#include <limits.h>

// glibc non ha strlcpy
#ifdef __GNU_LIBRARY__
    #include "../include/safe_str/strlcpy.h"
#else
    #include <string.h>
#endif



#include "../include/script.h"
#include "../include/search.h"


void print(edit* e)
{
    if (e->type == INSERTION)
    {
        printf("Insert %c", e->arg2);
    }
    else if (e->type == DELETION)
    {
        printf("Delete %c", e->arg1);
    }
    else
    {
        printf("Substitute %c for %c", e->arg2, e->arg1);
    }
    printf(" at %u\n", e->pos);
}


int main(void)
{
    edit* script;

    int distance1 = 0;
    unsigned int distance2 = 0;

    const char str1[] = "asdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdssssssddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdad";
    const char str2[] = "asdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdadsdadddddddddddddddreeeeeeeeeeeeeeeasdsdad";

    edit* ss = NULL;
    distance1 = levenshtein_matrix_calculate(ss, str1, strlen(str1), str2, strlen(str2));


    //distance2 = levdistOpt(str1, strlen(str1) + 1, str2, strlen(str2) + 1);

    printf("Distance is %d:\n", distance1);
    //printf("Distance is %d:\n", distance2);

    //for (int i = 0; i < distance1; i++)
    //{
    //    print(&script[i]);
    //}

    searchPrint("C:\\Users\\savo9\\html\\bc_s.png", "C:\\Users\\savo9\\html");

    free(script);

    return 0;
}

char* filename;

char* file1;
char* file2;
char* output;
char* inputfile;
char* dir;
int limit;
char *endptr;

int parseArgs(int argc, char* argv)
{
    if (strcmp(argv[1], "distance") == 0)
    {
        if (argc == 3)
        {
            memcpy(file1, argv[2], sizeof(argv[2])); // todo check
            memcpy(file2, argv[3], sizeof(argv[3]));
        }
        else if (argc == 4)
        {
            memcpy(file1, argv[2], sizeof(argv[2])); // todo check
            memcpy(file2, argv[3], sizeof(argv[3]));

            memcpy(output, argv[4], sizeof(argv[3]));
        }
        else
        {
            perror("Numero di argomenti errato");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "search") == 0)
    {
        if (argc == 3)
        {
            memcpy(inputfile, argv[2], sizeof(argv[2]));
        }
        else
        {
            perror("Numero di argomenti errato");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(argv[1], "searchall") == 0)
    {
        if (argc == 4)
        {
            strlcpy(inputfile, argv[2], sizeof(argv[2]));
            memcpy(dir, argv[3], sizeof(argv[2]));

            char* limitStr = (char *) argv[4];

            long int val = strtol(limitStr, &endptr, 10);

            if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
            {
                perror("strtol");
                exit(EXIT_FAILURE);
            }

            if (endptr == limitStr)
            {
                fprintf(stderr, "No digits were found\n");
                perror("Numero di argomenti errato");
                exit(EXIT_FAILURE);
            }

        }
        else
        {
            perror("Numero di argomenti errato");
            exit(EXIT_FAILURE);
        }
    }
}



























