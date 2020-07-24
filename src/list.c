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
#include <stdlib.h>

#include "../include/list.h"


node* list_create(void* data, node* next)
{
    node* new_node = (node*) malloc(sizeof(node));
    if (!new_node)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->next = next;

    return new_node;
}


node* list_append(node* list, void* data)
{
    if (list == NULL)
        return NULL;
    /* go to the last node */
    node* curr = list;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    /* create a new node */
    node* new_node = list_create(data,NULL);
    curr->next = new_node;

    return list;
}


node* list_filter(node* list, comparison_f f, int op, long value)
{
    if (list == NULL)
    {
        return NULL;
    }
    if ( f(list->data, op, value) )
    {
        list->next = list_filter(list->next, f, op, value);
        return list;
    }
    else
    {
        node* next = list->next;
        free(list);
        list = NULL;

        return list_filter(next, f, op, value);
    }
}


void list_traverse(node* list, callback_t f)
{
    node* cursor = list;
    while (cursor != NULL)
    {
        f(cursor);
        cursor = cursor->next;
    }
}


void list_free(node* list)
{
    node* curr;
    node* tmp;

    if (list != NULL)
    {
        curr = list->next;
        list->next = NULL;
        while (curr != NULL)
        {
            tmp = curr->next;
            free(curr);
            curr = tmp;
        }
    }
}


int list_count(node* list)
{
    if (list == NULL)
    {
        return -1;
    }

    node* curr = list;
    int c = 0;
    while (curr != NULL)
    {
        c++;
        curr = curr->next;
    }

    return c;
}


