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
#include <errno.h>
#include <limits.h> // INT_MAX

#include "../include/list.h"
#include "../include/list_namedistance.h"

int list_namedistance_save_to_array(node* list, name_distance** arr)
{
    int cnt = list_count(list);
    if (cnt > 0)
    {
        /* alloc array to save to */
        *arr = (name_distance*) calloc(cnt, sizeof(name_distance));

        node* curr = list;
        int index = 0;

        /* for each item of the list */
        while (curr != NULL)
        {
            /* get payload struct */
            name_distance* nd = (name_distance*) (curr->data);
            int dist = nd->distance;

            /* alloc arr[i] */
            char* entry = malloc(sizeof(nd->filename));
            if (entry)
            {
                /* copy & detect truncation */
                if (strlcpy(entry, nd->filename, sizeof(nd->filename)) >= sizeof(nd->filename))
                {
                    return -1;
                }
            }

            /* set fields of arr[index] */
            (*arr)[index].distance = dist;
            strlcpy((*arr)[index].filename, entry, sizeof(nd->filename));

            curr = curr->next;
            index++;
        }

        return 0;
    }
    else
    {
        errno = EEMPTYLIST;
        return -1;
    }
}

int list_namedistance_min(node* list)
{
    int min = INT_MAX;

    while (list != NULL)
    {
        name_distance* data = (name_distance*) list->data;

        if (min > data->distance)
            min = data->distance;

        list = list->next;
    }

    return min;
}

void list_namedistance_print_name(node* node)
{
    name_distance* nd = (name_distance*) node->data;

    printf("%s\n", nd->filename);
}