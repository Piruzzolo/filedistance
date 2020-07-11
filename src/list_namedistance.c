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

#include "../include/list.h"
#include "../include/list_namedistance.h"

int list_namedistance_save_to_array(node* list, name_distance** arr)
{
    int cnt = list_count(list);
    if (cnt)
    {
        *arr = (name_distance*) calloc(cnt, sizeof(name_distance));
        node* cursor = list;
        int i = 0;
        while (cursor != NULL)
        {
            name_distance* nd = (name_distance*) (cursor->data);
            int dist = nd->distance;
            char* entry = malloc(sizeof(nd->filename));
            if (entry)
            {
                // copy & detect truncation
                if (strlcpy(entry, nd->filename, sizeof(nd->filename)) >= sizeof(nd->filename))
                {
                    // error
                    return -1;
                }
            }

            (*arr)[i].distance = dist;
            strlcpy((*arr)[i].filename, entry, sizeof(nd->filename));

            cursor = cursor->next;
            i++;
        }

        return 0;
    }
    else
    {
        return -1;
    }

}