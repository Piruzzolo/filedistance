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
node* create(void* data, node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;

    return new_node;
}

/*
    add a new node at the end of the list
*/
node* append(node* head, void* data)
{
    if(head == NULL)
        return NULL;
    /* go to the last node */
    node *cursor = head;
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    /* create a new node */
    node* new_node = create(data,NULL);
    cursor->next = new_node;

    return head;
}


node* filter_list(node* head, int op, long value, comparison_f f)
{
    if (head == NULL)
    {
        return NULL;
    }
    if (f(head->data, op, value))
    {
        head->next = filter_list(head->next, op, value, f);
        return head;
    }
    else
    {
        node* next = head->next;
        free(head);

        return filter_list(next, op, value, f);
    }
}

/*
    traverse the linked list
*/
void traverse_list(node* head, callback_t f)
{
    node* cursor = head;
    while (cursor != NULL)
    {
        f(cursor);
        cursor = cursor->next;
    }
}

int save_to_array(node* list, name_distance** arr)
{
    int cnt = count(list);
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

/*
    remove all element of the list
*/
void list_free(node *head)
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
int count(node *head)
{
    node *cursor = head;
    int c = 0;
    while(cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}


