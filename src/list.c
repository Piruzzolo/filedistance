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


/*
    create a new node
    initialize the data and next field

    return the newly created node
*/
node* list_create(void* data, node* next)
{
    node* new_node = (node*) malloc(sizeof(node));
    if (new_node == NULL)
    {
        printf("Error creating a new node.\n"); // todo
        exit(0);
    }

    new_node->data = data;
    new_node->next = next;

    return new_node;
}

/*
    add a new node at the end of the list
*/
node* list_append(node* list, void* data)
{
    if (list == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = list;
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    /* create a new node */
    node* new_node = list_create(data,NULL);
    cursor->next = new_node;

    return list;
}


node* list_filter(node* list, int op, long value, comparison_f f)
{
    if (list == NULL)
    {
        return NULL;
    }
    if (f(list->data, op, value))
    {
        list->next = list_filter(list->next, op, value, f);
        return list;
    }
    else
    {
        node* next = list->next;
        free(list);

        return list_filter(next, op, value, f);
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
    node* cursor;
    node* tmp;

    if (list != NULL)
    {
        cursor = list->next;
        list->next = NULL;
        while (cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}


int list_count(node* list)
{
    node* cursor = list;
    int c = 0;
    while (cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}


