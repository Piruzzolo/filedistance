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
node* list_append(node* head, void* data)
{
    if (head == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = head;
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    /* create a new node */
    node* new_node = list_create(data,NULL);
    cursor->next = new_node;

    return head;
}


node* list_filter(node* head, int op, long value, comparison_f f)
{
    if (head == NULL)
    {
        return NULL;
    }
    if (f(head->data, op, value))
    {
        head->next = list_filter(head->next, op, value, f);
        return head;
    }
    else
    {
        node* next = head->next;
        free(head);

        return list_filter(next, op, value, f);
    }
}

/*
    traverse the linked list
*/
void list_traverse(node* head, callback_t f)
{
    node* cursor = head;
    while (cursor != NULL)
    {
        f(cursor);
        cursor = cursor->next;
    }
}

/*
    remove all element of the list
*/
void list_free(node* head)
{
    node *cursor, *tmp;

    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}

/*
    return the number of elements in the list
*/
int list_count(node* head)
{
    node* cursor = head;
    int c = 0;
    while(cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}


